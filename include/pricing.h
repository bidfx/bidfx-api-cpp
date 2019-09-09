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

#ifndef PUBLIC_API_CPP_PRICING_H_
#define PUBLIC_API_CPP_PRICING_H_

#include <include/provider_status_event.h>
#include <include/subscription_status_event.h>
#include <include/price_update_event.h>
#include <functional>
#include <include/bulk_subscriber.h>
#include <memory>
#include <chrono>

namespace bidfx_public_api
{

using bidfx_public_api::BulkSubscriber;
using bidfx_public_api::PriceUpdateEvent;
using bidfx_public_api::ProviderStatusEvent;
using bidfx_public_api::SubscriptionStatusEvent;

/**
 * This interface provides access to the realtime pricing subsystem.
 * @author Frazer Bennett Wilford
 */
class Pricing
{
public:
    /**
     * Sets an event callback function for price update events.
     * @param callback the function pointer to the event callback.
     */
    virtual Pricing& SetPriceUpdateEventCallback(std::function<void(std::shared_ptr<PriceUpdateEvent>)>& callback) = 0;

    /**
     * Sets an event callback function for provider status events.
     * @param callback the function pointer to the event callback.
     */
    virtual Pricing& SetProviderStatusEventCallback(std::function<void(std::shared_ptr<ProviderStatusEvent>)>& callback) = 0;

    /**
     * Sets an event callback function for subscription status events.
     * @param callback the function pointer to the event callback.
     */
    virtual Pricing& SetSubscriptionStatusEventCallback(std::function<void(std::shared_ptr<SubscriptionStatusEvent>)>& callback) = 0;

    /*
     * Returns a vector containing a properties state for each of the providers for this pricing session
     */
    virtual std::vector<ProviderProperties> GetProviderProperties() = 0;

    /**
     * Wait until the session is ready for use.
     * @param duration the maximum duration to wait for.
     * @throws InterruptedException if the session is not ready within the duration.
     */
    virtual std::unique_ptr<BulkSubscriber> WaitForSubscriber(std::chrono::milliseconds duration) = 0;

    /**
     * Wait until the session is ready for use.
     * @throws InterruptedException if the session is not ready within a few sesonds.
     */
    virtual std::unique_ptr<BulkSubscriber> WaitForSubscriber() = 0;

    /**
     * Stops the price provider and free up its network and file resources.
     */
     virtual void Stop() = 0;

    /*
     * Whether the pricing session is ready yet
     */
    virtual bool IsReady() = 0;
};

}

#endif //PUBLIC_API_CPP_PRICING_H_
