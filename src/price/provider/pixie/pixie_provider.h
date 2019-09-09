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

#ifndef PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_PIXIE_PROVIDER_PLUGIN_H_
#define PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_PIXIE_PROVIDER_PLUGIN_H_

#include <atomic>
#include <thread>
#include <chrono>
#include "include/user_info.h"
#include "src/price/provider/provider_consumer.h"
#include "src/price/provider/abstract_provider.h"
#include "src/price/provider/pixie/messages/welcome_message.h"
#include "src/tools/byte_buffer.h"
#include "src/price/provider/pixie/fields/data_dictionary.h"
#include "src/price/provider/pixie/messages/grant_message.h"
#include "src/price/provider/pixie/messages/data_dictionary_message.h"
#include "src/price/provider/pixie/messages/price_sync_decoder.h"
#include "src/tools/guid.h"
#include "src/tools/network/mbed_ssl_client.h"
#include "pixie_protocol_options.h"
#include "subject_set_register.h"
#include "ack_data.h"
#include "src/tools/blocking_queue.h"
#include "lib/logging/spdlog/spdlog.h"
#include "lib/logging/spdlog/logger.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::GUID;
using bidfx_public_api::tools::MBedSSLClient;
using bidfx_public_api::price::provider::AbstractProvider;
using bidfx_public_api::price::provider::ProviderConsumer;
using bidfx_public_api::price::provider::ProviderProperties;
using bidfx_public_api::price::pixie::DataDictionaryMessage;
using bidfx_public_api::tools::SSLClient;
using bidfx_public_api::tools::InputStream;
using bidfx_public_api::tools::OutputStream;
using bidfx_public_api::tools::ByteBuffer;
using bidfx_public_api::tools::BlockingQueue;


class PixieProvider : public AbstractProvider
{
private:
    static std::shared_ptr<spdlog::logger> Log;

    std::atomic_int writer_thread_count_ = 0;
    PixieProtocolOptions protocol_options_;
    PriceSyncDecoder price_sync_decoder_;
    int negotiated_version_;

    std::unique_ptr<IDataDictionary> data_dictionary_;
    BlockingQueue<AckData> ack_queue_;

    SubjectSetRegister subject_set_register_;

    UserInfo* user_info_;

    std::chrono::milliseconds subscription_interval_ = std::chrono::milliseconds(200);
    std::chrono::high_resolution_clock::time_point last_subscription_check_time_;

    std::chrono::high_resolution_clock::time_point last_write_time_;

    bool disconnection_triggered_ = false;

    void Login(InputStream& in, OutputStream& out, PixieProtocolOptions& options, SSLClient& ssl_client);
    void WriteProtocolSignature(OutputStream& out, std::string url);
    WelcomeMessage ReadWelcomeMessage(InputStream& in, SSLClient& ssl_client);
    GrantMessage ReadGrantMessage(InputStream& in, SSLClient& ssl_client);
    ByteBuffer ReadMessageFrame(InputStream& in, SSLClient& ssl_client);
    void CheckType(ByteBuffer& message_frame, const unsigned char& expected_type);
    void MainLoop(InputStream& in, OutputStream& out, SSLClient& ssl_client);
    void HandleNextMessage(InputStream& in, SSLClient& ssl_client);
    void HandlePriceSync(PriceSync& price_sync);
    void OnDataDictionary(DataDictionaryMessage& dict_message);
    void StartWriter(OutputStream& out, SSLClient& ssl_client);
    void HandleAcksAndSendSubscriptionSyncsAndHeartbeats(OutputStream& out, int thread_num);
    std::unique_ptr<AckData> PollNextAck();
    void PeriodicallyCheckSubscriptions(OutputStream& out);
    void CheckAndSendSubscriptions(OutputStream& out);
    void TryAndWriteHeartbeat(OutputStream& out);
    void OnConnectionError();

protected:
    void InitiatePriceServerConnection(SSLClient& ssl_client) override;

public:
    PixieProvider(UserInfo *user_info);

    void Start() override;

    void Subscribe(Subject subject, bool refresh) override;
    void Subscribe(Subject subject) override;
    void Unsubscribe(Subject& subject) override;

    Provider::Status GetStatus() override;

    void WriteMessage(const PixieMessage& message, OutputStream& out);
    void Write(ByteBuffer& buffer, OutputStream& out);

    SubjectSetRegister& GetSubjectSetRegister();

private:
    class PriceSyncVisitor : public Syncable
    {
    private:
        std::vector<Subject>& subject_set_;
        PixieProvider& pixie_provider;
        void HandlePriceUpdateEvent(uint32_t sid, std::map<std::string, PriceField>& price, SubjectSetRegister& subjectSetRegister, bool replaceAllFields);

    public:
        PriceSyncVisitor(PixieProvider &pixie_provider, std::vector<Subject> &subject_set);

        void PriceImage(uint32_t sid, std::map<std::string, PriceField> price) override;

        void PriceUpdate(uint32_t sid, std::map<std::string, PriceField> price) override;

        void PriceStatus(uint32_t sid, SubscriptionStatus status, std::string explanation) override;
    };

    IDataDictionary& GetDataDictionary();
};

} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_PIXIE_PROVIDER_PLUGIN_H_
