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

#ifndef BIDFX_PUBLIC_API_ABSTRACT_PROVIDER_H_
#define BIDFX_PUBLIC_API_ABSTRACT_PROVIDER_H_

#include "include/subscription_status.h"
#include "include/user_info.h"
#include "src/tools/network/tunnel_connector.h"
#include "src/price/provider/pixie/run-state.h"
#include "src/tools/countdown_latch.h"
#include "include/price/provider/provider.h"
#include "provider_consumer.h"
#include "include/price/price_field.h"
#include "lib/logging/spdlog/spdlog.h"
#include "lib/logging/spdlog/logger.h"
#include "src/price/subscription.h"

namespace bidfx_public_api::price::provider
{

using bidfx_public_api::tools::TunnelConnector;
using bidfx_public_api::tools::SSLClient;
using bidfx_public_api::tools::CountdownLatch;
using bidfx_public_api::price::pixie::RunState;
using bidfx_public_api::price::PriceField;

/**
 * @author Frazer Bennett Wilford
 */
class AbstractProvider : public Provider
{
private:
    static std::shared_ptr<spdlog::logger> Log;

    UserInfo* user_info_;
    TunnelConnector tunnel_connector_;
    std::unique_ptr<SSLClient> ssl_client_;

    std::atomic<RunState> run_state_ = RunState::INITIAL;
    CountdownLatch latch;

protected:
    std::function<void(const Subject&, const std::map<std::string, PriceField> & , bool)> price_update_callback_;
    std::function<void(const Subject&, SubscriptionStatus, std::string)> subscription_status_callback_;
    std::function<void(std::shared_ptr<ProviderStatusEvent>)> provider_status_event_callback_;

    Status provider_status_;
    std::string provider_status_text_;
    std::string name_;

    void ContinuouslyConnectLoop();
    void CreateConnection();

    void PublishPriceUpdate(const Subject& subject, const std::map<std::string, PriceField>& price_update,
                            bool replace_all_fields);

    void PublishSubscriptionStatus(const Subject& subject, SubscriptionStatus status, std::string& explanation);

    void Start(std::function<void()> runnable);

    void SetProviderStatus(Status status, std::string text);

    virtual void InitiatePriceServerConnection(SSLClient& ssl_client_) = 0;

    bool IsRunning();

public:
    AbstractProvider(UserInfo* user_info, std::string service);

    ~AbstractProvider();

    /**
     * Starts the provider listening for price updates.
     */
    virtual void Start() override = 0;

    /**
     * Stops the price provider and free up its network and file resources.
     */
    void Stop() override;

    ProviderProperties GetProperties() override;

    /**
     * Gets the name of the price provider.
     */
    std::string GetName() override;

    /**
     * Waits until the provider is ready.
     * @param duration the duration to wait for
     * @throws InterruptedException if interrupted while waiting.
     */
    void WaitUntilReady(std::chrono::milliseconds duration) override;

    void SetProviderStatusEventCallback(std::function<void(std::shared_ptr<ProviderStatusEvent>)> callback) override;

    void SetPriceUpdateEventCallback(
            std::function<void(const Subject&, const std::map<std::string, PriceField>&, bool)> callback) override;

    void SetSubscriptionStatusEventCallback(
            std::function<void(const Subject&, SubscriptionStatus, std::string)> callback) override;
};


}

#endif //BIDFX_PUBLIC_API_ABSTRACT_PROVIDER_H_
