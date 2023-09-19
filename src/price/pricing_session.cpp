/**  Copyright 2019 BidFX

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

            http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */

#include <iostream>
#include "include/tools/logger_factory.h"
#include "src/price/pricing_session.h"
#include "src/price/pricing_subscriber.h"

namespace bidfx_public_api::price
{

using bidfx_public_api::tools::LoggerFactory;

std::shared_ptr<spdlog::logger> PricingSession::Log = nullptr;

PricingSession::PricingSession(UserInfo* user_info)
{
    if (Log == nullptr)
    {
        Log = LoggerFactory::GetLogger("PricingSession");
    }

    user_info_ = user_info;
    pricing_subscriber_ = new PricingSubscriber(this, user_info);
    pixie_provider_ = new PixieProvider(user_info);
    running_ = ATOMIC_VAR_INIT(false);
    PrepareCallbacks();
}

PricingSession::~PricingSession()
{
    Stop();
    delete(pricing_subscriber_);
    delete(pixie_provider_);
}

void PricingSession::PrepareCallbacks()
{
    auto PriceUpdateEventCallback = [this] (const Subject& subject, const std::map<std::string, PriceField>& price_update, bool replace_all_fields) {OnPriceUpdate(subject, price_update, replace_all_fields);};
    pixie_provider_->SetPriceUpdateEventCallback(PriceUpdateEventCallback);
    auto ProviderStatusEventCallback = [this] (std::shared_ptr<ProviderStatusEvent> event) {OnProviderStatusUpdate(event);};
    pixie_provider_->SetProviderStatusEventCallback(ProviderStatusEventCallback);
    auto SubscriptionStatusCallback = [this] (const Subject& subject, SubscriptionStatus status, std::string reason) {OnSubscriptionStatus(subject, status, reason);};
    pixie_provider_->SetSubscriptionStatusEventCallback(SubscriptionStatusCallback);
}

Pricing& PricingSession::SetPriceUpdateEventCallback(std::function<void(std::shared_ptr<PriceUpdateEvent>)>& callback)
{
    price_update_callback_.push_back(callback);
    return *this;
}

Pricing& PricingSession::SetProviderStatusEventCallback(
        std::function<void(std::shared_ptr<ProviderStatusEvent>)>& callback)
{
    provider_status_update_callback_.push_back(callback);
    return *this;
}

Pricing& PricingSession::SetSubscriptionStatusEventCallback(
        std::function<void(std::shared_ptr<SubscriptionStatusEvent>)>& callback)
{
    subscription_status_callback_.push_back(callback);
    return *this;
}

void PricingSession::OnPriceUpdate(const Subject& subject, const std::map<std::string, PriceField>& price_update,
                                   bool replace_all_fields)
{
    if(!running_)
    {
        return;
    }
    auto subscription_it = subscriptions_.find(subject);
    if (subscription_it == subscriptions_.end())
    {
        return;
    }
    Subscription& subscription = subscription_it->second;
    if (SubscriptionStatus::OK != subscription.GetStatus())
    {
        OnSubscriptionStatus(subscription.GetSubject(), SubscriptionStatus::OK, "Received price update.");
    }

    subscription.MergePriceMap(price_update, replace_all_fields);

    std::shared_ptr<PriceUpdateEvent> event = std::make_shared<PriceUpdateEvent>(subject, subscription.GetAllPriceFields(), price_update);
    try
    {
        for (auto callback : price_update_callback_)
        {
            callback(event);
        }
    }
    catch (std::exception &e)
    {
        Log->warn("Error handling price update {}", e.what());
        throw e;
    }
}

void PricingSession::OnProviderStatusUpdate(std::shared_ptr<ProviderStatusEvent> event)
{
    if (!running_)
    {
        return;
    }
    if (event->GetProviderProperties().GetStatus() == Provider::Status::READY)
    {
        ResubscribeToAllOn(pixie_provider_);
    }

    try
    {
        for (auto callback : provider_status_update_callback_)
        {
            callback(event);
        }
    }
    catch (std::exception &e)
    {
        Log->warn("Error handling provider status {}", e.what());
        throw e;
    }
}

void PricingSession::OnSubscriptionStatus(const Subject& subject, SubscriptionStatus status, std::string reason)
{
    if (!running_)
    {
        return;
    }
    auto subscription_it = subscriptions_.find(subject);
    if (subscription_it != subscriptions_.end())
    {
        subscription_it->second.SetStatus(status);
        subscription_it->second.ClearPriceFields();
    }

    std::shared_ptr<SubscriptionStatusEvent> event = std::make_shared<SubscriptionStatusEvent>(subject, status, reason);
    try
    {
        for (auto callback : subscription_status_callback_)
        {
            callback(event);
        }
    }
    catch (std::exception &e)
    {
        Log->warn("Error handling price status {}", e.what());
        throw e;
    }
}

void PricingSession::ResubscribeToAllOn(Provider* provider)
{
    Log->info("Refreshing all subscribed subjects");

    for (auto subject_subscription : subscriptions_)
    {
        provider->Subscribe(subject_subscription.first, true);
    }
}

void PricingSession::ResubscribeAll()
{
    if (pixie_provider_->GetStatus() == Provider::Status::READY)
    {
        ResubscribeToAllOn(pixie_provider_);
    }
}

std::unique_ptr<BulkSubscriber> PricingSession::WaitForSubscriber(std::chrono::milliseconds duration)
{
    if (running_.exchange(true))
    {
        // Pricing session was already running
        return std::make_unique<PricingSubscriber>(this, user_info_);
    }
    pixie_provider_->Start();
    pixie_provider_->WaitUntilReady(duration);
    return std::make_unique<PricingSubscriber>(this, user_info_);
}

std::unique_ptr<BulkSubscriber> PricingSession::WaitForSubscriber()
{
    return WaitForSubscriber(std::chrono::milliseconds(1000));
}

void PricingSession::Stop()
{
    if (!running_.exchange(false))
    {
        return;
    }
    Log->info("Stopping");
    subscriptions_.clear();
    pixie_provider_->Stop();
    return;
}

void PricingSession::Subscribe(Subject subject, bool refresh)
{
    Log->info("subscribing to {}", subject.ToString());

    if (subscriptions_.find(subject) == subscriptions_.end())
    {
        subscriptions_.emplace(subject, subject);
    }

    pixie_provider_->Subscribe(std::move(subject), refresh);
}

void PricingSession::Subscribe(Subject subject)
{
    Subscribe(std::move(subject), false);
}

void PricingSession::Unsubscribe(Subject& subject)
{
    Log->info("unsubscribing from {}", subject.ToString());
    subscriptions_.erase(subject);
    pixie_provider_->Unsubscribe(subject);
}

bool PricingSession::IsReady()
{
    return pixie_provider_->GetStatus() == Provider::Status::READY;
}

std::vector<ProviderProperties> PricingSession::GetProviderProperties()
{
    return {pixie_provider_->GetProperties()};
}

}