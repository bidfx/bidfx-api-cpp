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

#ifndef PUBLIC_API_CPP_PRICE_PRICING_SESSION_H_
#define PUBLIC_API_CPP_PRICE_PRICING_SESSION_H_

#include "include/pricing.h"
#include "include/price/provider/provider.h"
#include "src/price/provider/pixie/pixie_provider.h"
#include "include/subscriber.h"
#include "pricing_subscriber.h"
#include "lib/logging/spdlog/spdlog.h"
#include "lib/logging/spdlog/logger.h"
#include <set>

namespace bidfx_public_api::price
{

using bidfx_public_api::price::pixie::PixieProvider;
using bidfx_public_api::price::provider::Provider;

class PricingSubscriber;

class PricingSession : public Subscriber, public Pricing
{
private:
    static std::shared_ptr<spdlog::logger> Log;

    UserInfo* user_info_;
    PricingSubscriber* pricing_subscriber_;
    Provider* pixie_provider_;
    std::vector<std::function<void (std::shared_ptr<ProviderStatusEvent>)>> provider_status_update_callback_;
    std::vector<std::function<void (std::shared_ptr<PriceUpdateEvent>)>> price_update_callback_;
    std::vector<std::function<void (std::shared_ptr<SubscriptionStatusEvent>)>> subscription_status_callback_;
    std::atomic_bool running_;

    std::map<Subject, Subscription> subscriptions_;


    void OnPriceUpdate(const Subject& subject, const std::map<std::string, PriceField>& price_update,
                       bool replace_all_fields);
    void OnProviderStatusUpdate(std::shared_ptr<ProviderStatusEvent> event);
    void OnSubscriptionStatus(const Subject& subject, SubscriptionStatus status, std::string reason);


    void ResubscribeToAllOn(Provider* provider);
    void PrepareCallbacks();

public:
    explicit PricingSession(UserInfo* user_info);
    ~PricingSession();

    Pricing& SetPriceUpdateEventCallback(std::function<void(std::shared_ptr<PriceUpdateEvent>)>& callback) override;
    Pricing& SetProviderStatusEventCallback(std::function<void(std::shared_ptr<ProviderStatusEvent>)>& callback) override;
    Pricing& SetSubscriptionStatusEventCallback(std::function<void(std::shared_ptr<SubscriptionStatusEvent>)>& callback) override;

    std::unique_ptr<BulkSubscriber> WaitForSubscriber(std::chrono::milliseconds duration) override;
    std::unique_ptr<BulkSubscriber> WaitForSubscriber() override;
    void Stop() override;

    std::vector<ProviderProperties> GetProviderProperties() override;

    void ResubscribeAll();

    void Subscribe(Subject subject, bool refresh) override;
    void Subscribe(Subject subject) override;
    void Unsubscribe(Subject& subject) override;

    bool IsReady() override;
};

} // bidfx_public_api::price


#endif //PUBLIC_API_CPP_PRICE_PRICING_SESSION_H_
