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
#include <thread>
#include <sstream>
#include "include/price_update_event.h"
#include "include/subscription_status_event.h"
#include "include/exceptions/timeout_exception.h"
#include "include/tools/logger_factory.h"
#include "src/price/subscription.h"
#include "src/price/provider/abstract_provider.h"
#include "abstract_provider.h"
#include "include/subscription_status.h"
#include "provider_status_event_impl.h"

namespace bidfx_public_api::price::provider
{

using bidfx_public_api::exceptions::TimeoutException;

using bidfx_public_api::tools::LoggerFactory;

std::shared_ptr<spdlog::logger> AbstractProvider::Log = nullptr;

AbstractProvider::AbstractProvider(UserInfo* user_info, std::string service) : tunnel_connector_(TunnelConnector(*user_info, service)), latch(CountdownLatch(1))
{
    if (Log == nullptr)
    {
        Log = LoggerFactory::GetLogger("AbstractProvider");
    }

    Log->info("creating provider: " + GetName());

    provider_status_ = Status::UNAVAILABLE;
    user_info_ = user_info;

    name_ = "[]";
    run_state_ = RunState::INITIAL;
}

AbstractProvider::~AbstractProvider()
{
    Stop();
}

void AbstractProvider::ContinuouslyConnectLoop()
{
    while (IsRunning())
    {
        CreateConnection();
        if (IsRunning())
        {
            Log->info("will reconnect in 10 seconds");
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
}

void AbstractProvider::CreateConnection()
{
    try
    {
        ssl_client_ = tunnel_connector_.Connect(GetProtocolOptions().GetHeartbeatInterval() * 2);
        InitiatePriceServerConnection(ssl_client_);
        ssl_client_->Close();
    }
    catch(std::exception &e)
    {
        Log->error("create connection had error {}", e.what());
    }
}

void AbstractProvider::Start(std::function<void()> runnable)
{
    RunState initial = RunState::INITIAL;
    if (run_state_.compare_exchange_strong(initial, RunState::RUNNING))
    {
        Log->info("starting price provider {}", name_);
        SetProviderStatus(Status::DOWN, "starting price provider");
        runnable();
    }
    else
    {
        Log->warn("price provider {} is already running", name_);
    }
}

void AbstractProvider::Stop()
{
    RunState running = RunState::RUNNING;
    if (run_state_.compare_exchange_strong(running, RunState::STOPPED))
    {
        Log->info("stopping price provider " +  name_);
        ssl_client_->Close();
        SetProviderStatus(Status::CLOSED, "closed by request");
    }
}

void AbstractProvider::PublishPriceUpdate(const Subject& subject, const std::map<std::string, PriceField>& price_update,
                                          bool replace_all_fields)
{
    price_update_callback_(subject, price_update, replace_all_fields);
}

void AbstractProvider::PublishSubscriptionStatus(const Subject& subject, SubscriptionStatus status,
                                                 std::string& explanation)
{
    subscription_status_callback_(subject, status, explanation);
}

void AbstractProvider::SetProviderStatus(Provider::Status status, std::string text)
{
    if (provider_status_ == Status::CLOSED)
    {
        Log->debug("event ignored by closed provider");
    }
    else
    {
        Status previous_status = provider_status_;
        provider_status_ = status;
        provider_status_text_ = text;

        Log->info("price provider {} is {} {}", name_, provider_status_, provider_status_text_);

        if (previous_status != status && status != Status::CLOSED)
        {
            if (status == Status::READY)
            {
                latch.CountDown();
            }
        }

        ProviderProperties provider_properties = ProviderProperties(provider_status_, name_, provider_status_text_);
        std::shared_ptr<ProviderStatusEvent> new_provider_event = std::make_shared<ProviderStatusEventImpl>(provider_properties);

        provider_status_event_callback_(new_provider_event);
    }
}

void AbstractProvider::WaitUntilReady(std::chrono::milliseconds duration)
{
    if (!latch.Await(duration))
    {
        std::stringstream ss;
        ss << "Timeout Exception: Timeout of " << duration.count() << " milliseconds exceeded while trying to connect";
        throw TimeoutException(ss.str());
    }
}

void AbstractProvider::SetProviderStatusEventCallback(
        std::function<void(std::shared_ptr<ProviderStatusEvent>)> callback)
{
    provider_status_event_callback_ = callback;
}

void AbstractProvider::SetPriceUpdateEventCallback(
        std::function<void(const Subject&, const std::map<std::string, PriceField>&, bool)> callback)
{
    price_update_callback_ = callback;
}

void AbstractProvider::SetSubscriptionStatusEventCallback(
        std::function<void(const Subject&, SubscriptionStatus, std::string)> callback)
{
    subscription_status_callback_ = callback;
}

ProviderProperties AbstractProvider::GetProperties()
{
    return ProviderProperties(provider_status_, name_, provider_status_text_);
}

bool AbstractProvider::IsRunning()
{
    return run_state_ == RunState::RUNNING;
}

std::string AbstractProvider::GetName()
{
    return name_;
}

}