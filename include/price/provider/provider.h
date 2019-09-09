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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PROVIDER_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PROVIDER_H_

#include "include/subscriber.h"
#include "include/price_update_event.h"
#include "include/subscription_status_event.h"
#include <functional>
#include <chrono>
#include <map>
#include "spdlog/fmt/ostr.h"

namespace  bidfx_public_api
{
class ProviderStatusEvent;
}

namespace bidfx_public_api::price::provider
{

class ProviderProperties;

/**
 * @author Frazer Bennett Wilford
 */
class Provider : public Subscriber
{
public:
    enum Status
    {
        READY,
        DISABLED,
        DOWN,
        UNAVAILABLE,
        INVALID,
        CLOSED
    };

    /**
     * Gets the name of the price provider.
     */
    virtual std::string GetName() = 0;

    /**
     * Starts the provider listening for price updates.
     */
    virtual void Start() = 0;

    /**
     * Stops the price provider and free up its network and file resources.
     */
    virtual void Stop() = 0;

    /**
     * Waits until the provider is ready.
     * @param duration the duration to wait for
     * @throws InterruptedException if interrupted while waiting.
     */
    virtual void WaitUntilReady(std::chrono::milliseconds duration) = 0;

    virtual Status GetStatus() = 0;

    virtual ProviderProperties GetProperties() = 0;

    /**
     * Sets the callback method for when the provider status changes
     * @param function to be called
     */
    virtual void SetProviderStatusEventCallback(std::function<void(std::shared_ptr<ProviderStatusEvent>)> callback) = 0;

    /**
     * Sets the callback method for price update events.
     * @param function to be called
     */
    virtual void SetPriceUpdateEventCallback(
            std::function<void(const Subject& , const std::map<std::string, PriceField>& , bool)> callback) = 0;

    /**
     * Sets the callback method for subscription status events.
     * @param function to be called
     */
     virtual void SetSubscriptionStatusEventCallback(
            std::function<void(const Subject& , SubscriptionStatus, std::string)> callback) = 0;
};

static std::map<Provider::Status, std::string> provider_status_map = {
        {Provider::Status::READY, "READY"},
        {Provider::Status::DISABLED, "DISABLED"},
        {Provider::Status::DOWN, "DOWN"},
        {Provider::Status::UNAVAILABLE, "UNAVAILABLE"},
        {Provider::Status::INVALID, "INVALID"},
        {Provider::Status::CLOSED, "CLOSED"},
};

inline std::ostream& operator<<(std::ostream& os, const Provider::Status& s)
{
    return os << provider_status_map[s];
}

}
#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PROVIDER_H_
