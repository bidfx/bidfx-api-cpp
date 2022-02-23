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

#include <string>
#include <iostream>
#include <sstream>
#include "api_example.h"
#include "include/public_api.h"
#include "include/exceptions/timeout_exception.h"
#include "include/tools/logger_factory.h"
#include "include/tools/java_time.h"

int main(int argc, char** argv)
{
    using namespace bidfx_public_api;

    ApiExample* api_example = new ApiExample();
    api_example->RunTest();
}

namespace bidfx_public_api
{

using bidfx_public_api::price::PricingSubscriber;
using bidfx_public_api::Tenor;
using bidfx_public_api::tools::Date;
using bidfx_public_api::tools::LoggerFactory;
using bidfx_public_api::exceptions::TimeoutException;

ApiExample::ApiExample()
{
    ConfigureLogging(spdlog::level::debug);
    logger_ = LoggerFactory::GetLogger("ApiExample");

    session_ = PublicApi::CreateSession();
    session_->SetHost("api.ld.bidfx.biz ").
            SetUsername("").
            SetPassword("").
            SetDefaultAccount("FX_ACCT").
            SetApplication("Example API").
            SetApplicationVersion("1.0").
            SetProductSerialNumber("TEST_PRODUCT_0012345");

    std::function<void(std::shared_ptr<PriceUpdateEvent>)> price_update_callback = [this] (std::shared_ptr<PriceUpdateEvent> event) { OnPriceUpdate(event); };
    std::function<void(std::shared_ptr<SubscriptionStatusEvent>)> subscription_callback = [this] (std::shared_ptr<SubscriptionStatusEvent> event) { OnSubscriptionStatus(event); };
    std::function<void(std::shared_ptr<ProviderStatusEvent>)> provider_callback = [this] (std::shared_ptr<ProviderStatusEvent> event) { OnProviderStatus(event); };

    pricing_ = std::unique_ptr<Pricing>(&session_->pricing().
            SetPriceUpdateEventCallback(price_update_callback).
            SetSubscriptionStatusEventCallback(subscription_callback).
            SetProviderStatusEventCallback(provider_callback));

    for (ProviderProperties& properties : pricing_->GetProviderProperties())
    {
        std::cout << properties.GetName() << " is " << properties.GetStatusString();
    }

    try
    {
        subscriber_ = pricing_->WaitForSubscriber(std::chrono::milliseconds(10000));
    }
    catch (TimeoutException& e)
    {
        std::cout << "[ERROR] failed to connect, timeout exceeded: " << e.what();
        throw e;
    }
}

void ApiExample::ConfigureLogging(spdlog::level::level_enum level)
{
    LoggerFactory::AddStdoutLogger(true);
    LoggerFactory::SetLogLevel(level);
}

void ApiExample::RunTest()
{
    logger_->info("pricing session is ready");

    DealableStreams({"RBCFX", "SSFX", "MSFX", "CSFX", "JPMCFX", "HSBCFX"});
    DealableQuotes({"RBSFX", "UBSFX", "NOMURAFX", "CITIFX"});

    std::this_thread::sleep_for(std::chrono::seconds(15));

    logger_->info("Finishing example");
}

void ApiExample::DealableStreams(std::vector<std::string> liquidity_providers)
{
    for (std::string lp : liquidity_providers)
    {
        Subject subject1 = subscriber_->Subjects().Fx().Stream().Spot()
                .LiquidityProvider(lp)
                .CurrencyPair("EURUSD")
                .Currency("EUR")
                .Quantity(1230080.11)
                .CreateSubject();

        subscriber_->Subscribe(subject1);
        //subscriber_->Unsubscribe(subject1);

        Subject subject2 = subscriber_->Subjects().Fx().Stream().Forward()
                .LiquidityProvider(lp)
                .CurrencyPair("EURUSD")
                .Currency("EUR")
                .Quantity(1000000)
                .Tenor(Tenor::IN_1_MONTH)
                .CreateSubject();

        subscriber_->Subscribe(subject2);
        //subscriber_->Unsubscribe(subject2);

        Subject subject3 = subscriber_->Subjects().Fx().Stream().Ndf()
                .LiquidityProvider(lp)
                .CurrencyPair("USDKRW")
                .Currency("USD")
                .Quantity(2000000)
                .Tenor(Tenor::IN_1_YEAR)
                .FixingDate(new Date(2019, 12, 10))
                .SettlementDate(new Date(2019, 12, 12))
                .CreateSubject();

        subscriber_->Subscribe(subject3);
        //subscriber_->Unsubscribe(subject3);
    }
}

void ApiExample::DealableQuotes(std::vector<std::string> liquidity_providers)
{
    for (std::string lp : liquidity_providers)
    {
        subscriber_->Subscribe(subscriber_->Subjects().Fx().Quote().Spot()
                .LiquidityProvider(lp)
                .CurrencyPair("EURUSD")
                .Currency("EUR")
                .Quantity(1000000.11)
                .CreateSubject());

        subscriber_->Subscribe(subscriber_->Subjects().Fx().Stream().Forward()
                .LiquidityProvider(lp)
                .CurrencyPair("EURUSD")
                .Currency("EUR")
                .Quantity(1000000)
                .Tenor(Tenor::IN_1_MONTH)
                .CreateSubject());

        subscriber_->Subscribe(subscriber_->Subjects().Fx().Stream().Swap()
                .LiquidityProvider(lp)
                .CurrencyPair("CHFJPY")
                .Currency("CHF")
                .NearQuantity(15000000)
                .NearTenor(Tenor::IN_3_MONTHS)
                .FarQuantity(15000000)
                .FarTenor(Tenor::IN_1_YEAR)
                .CreateSubject());

        subscriber_->Subscribe(subscriber_->Subjects().Fx().Stream().Nds()
                .LiquidityProvider(lp)
                .CurrencyPair("USDCNY")
                .Currency("USD")
                .NearQuantity(10000000)
                .NearTenor(Tenor::IN_1_YEAR)
                .FarQuantity(8500000)
                .FarTenor(Tenor::IN_3_YEARS)
                .CreateSubject());
    }
}

void ApiExample::OnPriceUpdate(std::shared_ptr<PriceUpdateEvent> event)
{
    std::stringstream ss;
    ss << "price: " << event->GetSubject() << std::endl;
    for(const auto& [k, v] : event->GetAllPriceFields())
    {
        ss << " " << k << "=" << v;
    }
    ss << std::endl;
    logger_->info(ss.str());
}

void ApiExample::OnSubscriptionStatus(std::shared_ptr<SubscriptionStatusEvent> event)
{
    logger_->info("Subscription Status Update: {} -> {}: {}", event->GetSubject(), event->GetStatus(), event->GetExplanation());
}

void ApiExample::OnProviderStatus(std::shared_ptr<ProviderStatusEvent> event)
{
    logger_->info("provider Status Update: {}", event->Describe());
}

}
