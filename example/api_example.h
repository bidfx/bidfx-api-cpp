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

#ifndef BIDFX_PUBLIC_API_API_EXAMPLE_H
#define BIDFX_PUBLIC_API_API_EXAMPLE_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "src/price/pricing_session.h"
#include "include/bulk_subscriber.h"
#include "include/session.h"
#include "lib/logging/spdlog/spdlog.h"
#include "lib/logging/spdlog/logger.h"

namespace bidfx_public_api
{

using bidfx_public_api::price::PricingSession;
using bidfx_public_api::BulkSubscriber;
using bidfx_public_api::Session;

class ApiExample
{
private:
    std::unique_ptr<Session> session_;
    std::unique_ptr<Pricing> pricing_;
    std::unique_ptr<BulkSubscriber> subscriber_;
    std::shared_ptr<spdlog::logger> logger_;

    void ConfigureLogging(spdlog::level::level_enum level);

public:
    ApiExample();

    void RunTest();
    void DealableStreams(std::vector<std::string> liquidity_providers);
    void DealableQuotes(std::vector<std::string> liquidity_providers);

    void OnPriceUpdate(std::shared_ptr<PriceUpdateEvent> event);
    void OnSubscriptionStatus(std::shared_ptr<SubscriptionStatusEvent> event);
    void OnProviderStatus(std::shared_ptr<ProviderStatusEvent> event);
};

}

#endif //BIDFX_PUBLIC_API_API_EXAMPLE_H