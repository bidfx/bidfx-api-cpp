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
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <cstring>
#include <sstream>
#include <deque>
#include "include/price/field_name.h"
#include "include/exceptions/illegal_state_exception.h"
#include "src/tools/varint.h"
#include "src/price/provider/pixie/messages/welcome_message.h"
#include "src/price/provider/pixie/messages/pixie_message_type.h"
#include "src/price/provider/pixie/messages/login_message.h"
#include "include/public_api.h"
#include "src/price/provider/pixie/messages/grant_message.h"
#include "src/price/provider/pixie/messages/data_dictionary_message.h"
#include "src/price/provider/pixie/fields/data_dictionary_utils.h"
#include "src/price/provider/pixie/messages/price_sync.h"
#include "src/price/provider/pixie/messages/heartbeat_message.h"
#include "include/tools/logger_factory.h"
#include "include/tools/logging_utils.h"
#include "src/tools/name_cache.h"
#include "include/price/provider/provider.h"
#include "pixie_provider.h"
#include "pixie_version.h"
#include "ack_data.h"
#include "src/tools/optional.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::pixie::PixieMessageType;
using bidfx_public_api::tools::LoggerFactory;
using bidfx_public_api::tools::LoggingUtils;
using bidfx_public_api::tools::NameCache;
using bidfx_public_api::exceptions::IllegalStateException;

std::shared_ptr<spdlog::logger> PixieProvider::Log = nullptr;

PixieProvider::PixieProvider(UserInfo* user_info) : AbstractProvider(user_info, "static://highway")
{
    if (Log == nullptr)
    {
        Log = LoggerFactory::GetLogger("PixieProvider");
    }

    user_info_ = user_info;
    name_ = NameCache::GetDefault().CreateUniqueName("PixieProvider");
    protocol_options_ = PixieProtocolOptions();
}

void PixieProvider::Subscribe(Subject subject)
{
    Subscribe(std::move(subject), false);
}

void PixieProvider::Subscribe(Subject subject, bool refresh)
{
    subject_set_register_.Register(subject, refresh);
}

void PixieProvider::Unsubscribe(Subject& subject)
{
    Log->debug("price provider {} unsubscribing from {}", GetName(), subject.ToString());
    subject_set_register_.Unregister(subject);
}

void PixieProvider::Start()
{
    AbstractProvider::Start([this] () {
        std::thread thread(&PixieProvider::ContinuouslyConnectLoop, this);
        thread.detach();
    });
}

void PixieProvider::InitiatePriceServerConnection(SSLClient& ssl_client)
{
    try
    {
        InputStream& input_stream = ssl_client.GetInputStream();
        OutputStream& output_stream = ssl_client.GetOutputStream();
        Login(input_stream, output_stream, protocol_options_, ssl_client);
        SetProviderStatus(Status::READY, "");
        MainLoop(input_stream, output_stream, ssl_client);
    }
    catch (std::ios_base::failure& e)
    {
    }

    OnConnectionError();
}

void PixieProvider::Login(InputStream& in, OutputStream& out, PixieProtocolOptions& options, SSLClient& ssl_client)
{
    std::string protocol_signature = protocol_options_.GetProtocolSignature();
    WriteProtocolSignature(out, protocol_signature);
    WelcomeMessage welcome_message = ReadWelcomeMessage(in, ssl_client);
    Log->debug("after sending URL signature, received welcome: {}", welcome_message.ToString());
    negotiated_version_ = welcome_message.GetVersion();
    PixieVersion::ClientVerifyVersion(negotiated_version_);

    char* user_env = std::getenv("USERNAME");
    std::string user_env_string = user_env == NULL ? "" : user_env;

    LoginMessage message(user_info_->GetUsername(), user_info_->GetPassword(), user_env_string,
                            user_info_->GetApplication(), user_info_->GetApplicationVersion(), PublicApi::GetName(), PublicApi::GetVersion(), user_info_->GetProductSerialNumber());

    WriteMessage(message, out);
    GrantMessage grant_message = ReadGrantMessage(in, ssl_client);
    Log->debug("received grant {}", grant_message.ToString());
    data_dictionary_ = std::make_unique<ExtendableDataDictionary>();
    if (!grant_message.IsGranted()) throw std::runtime_error(grant_message.GetReason());
    Log->debug("authenticated with pixie server, PixieClient is logged in");
}

void PixieProvider::WriteProtocolSignature(OutputStream& out, std::string url)
{
    url = url + "\n";
    size_t len = url.length();
    const char* chars = url.c_str();
    unsigned char* bytes = new unsigned char[len];

    memcpy(bytes, chars, len);
    out.WriteBytes(bytes, len);
    delete[](bytes);
}

WelcomeMessage PixieProvider::ReadWelcomeMessage(InputStream& in, SSLClient& ssl_client)
{
    ByteBuffer welcome_frame = ReadMessageFrame(in, ssl_client);
    CheckType(welcome_frame, PixieMessageType::WELCOME);
    return WelcomeMessage(welcome_frame);
    //mLastMsgRecTime = System.currentTimeMillis();
}

GrantMessage PixieProvider::ReadGrantMessage(InputStream& in, SSLClient& ssl_client)
{
    ByteBuffer grant_frame = ReadMessageFrame(in, ssl_client);
    CheckType(grant_frame, PixieMessageType::GRANT);
    return GrantMessage(grant_frame);
    //mLastMsgRecTime = System.currentTimeMillis();
}

void PixieProvider::CheckType(ByteBuffer& message_frame, const unsigned char& expected_type)
{
    unsigned char received_type = message_frame.ReadByte();

    if (received_type != expected_type)
    {
        std::stringstream error_message;
        error_message << "received a message fo type " << received_type << " when expecting a message of type " << expected_type;
        throw std::invalid_argument(error_message.str());
    }
}

void PixieProvider::MainLoop(InputStream& in, OutputStream& out, SSLClient& ssl_client)
{
    Log->debug("entered main reading loop");
    StartWriter(out, ssl_client);

    try
    {
        while (IsRunning() && !disconnection_triggered_)
        {
            HandleNextMessage(in, ssl_client);
        }
    }
    catch (std::ios_base::failure& e)
    {
        Log->error("leaving the main loop, error: {}", e.what());
        throw e;
    }
}

void PixieProvider::HandleNextMessage(InputStream& in, SSLClient& ssl_client)
{
    ByteBuffer buffer = ReadMessageFrame(in, ssl_client);
    unsigned char msg_type = buffer.GetNextByte();

    if (msg_type == PixieMessageType::DATA_DICTIONARY)
    {
        DataDictionaryMessage dict_message(buffer);
        OnDataDictionary(dict_message);
    }
    else if (msg_type == PixieMessageType::PRICE_SYNC)
    {
        std::chrono::nanoseconds received_time_nanos = std::chrono::high_resolution_clock::now().time_since_epoch();
        std::chrono::milliseconds received_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        PriceSync price_sync = price_sync_decoder_.DecodePriceSync(buffer);
        HandlePriceSync(price_sync);
        ack_queue_.Push(AckData(price_sync.GetRevision(), price_sync.GetRevisionTime(), received_time.count(), received_time_nanos.count()));
    }
    else if (msg_type ==  PixieMessageType::HEARTBEAT)
    {
        Log->debug("Received a heartbeat message from the server");
    }
    else
    {
        Log->debug("Received message of type: {}", msg_type);
    }
}

void PixieProvider::HandlePriceSync(PriceSync& price_sync)
{
    uint32_t edition = price_sync.GetEdition();
    try
    {
        std::optional<std::vector<Subject>> subject_set = GetSubjectSetRegister().SubjectSetByEdition(edition);

        if (!subject_set)
        {
            std::stringstream ss;
            ss << "received PriceSync for edition " << edition << " but it's not in the SubjectSetRegister";
            throw IllegalStateException(ss.str());
        }
        else
        {
            PriceSyncVisitor visitor(*this, *subject_set);
            price_sync.Visit(GetDataDictionary(), visitor);
        }
    }
    catch (std::exception e)
    {
        std::stringstream ss;
        ss << "could not process PriceSync due to " << e.what();
        Log->warn(ss.str());

        throw e;
    }
}

ByteBuffer PixieProvider::ReadMessageFrame(InputStream& in, SSLClient& ssl_client)
{
    size_t frame_length = Varint::ReadU32(in);

    if (frame_length == 0)
    {
        std::stringstream error_message;
        error_message << "unexpected empty pixie message frame" << std::endl;
        throw std::ios_base::failure(error_message.str());
    }

    ByteBuffer message_frame(frame_length);
    unsigned char* frame_buffer = new unsigned char[frame_length];

    size_t total_read = 0;

    while (total_read < frame_length)
    {
        size_t got = in.ReadBytes(frame_buffer, frame_length - total_read);

        if (got == -1)
        {
            std::stringstream error_message;
            error_message << "end of message stream reached (perhaps the server closed the connection)" << std::endl;
            throw std::ios_base::failure(error_message.str());
        }

        total_read += got;
    }

    message_frame.WriteBytes(frame_buffer, frame_length);
    delete[] frame_buffer;

    return std::move(message_frame);
}

void PixieProvider::OnDataDictionary(DataDictionaryMessage& dict_message)
{
    if (dict_message.IsUpdate())
    {
        Log->info("updated current dictionary with new fields: {}", dict_message.ToString());
        auto field_defs = dict_message.GetFieldDefs();
        DataDictionaryUtils::AddAllFields(*data_dictionary_, field_defs);
    }
    else
    {
        Log->info("replacing dictionary with: {}", dict_message.ToString());
        data_dictionary_ = std::make_unique<ExtendableDataDictionary>();
        auto field_defs = dict_message.GetFieldDefs();
        DataDictionaryUtils::AddAllFields(*data_dictionary_, field_defs);
    }
}

void PixieProvider::StartWriter(OutputStream& out, SSLClient& ssl_client)
{
    writer_thread_count_++;
    int current_writer_number = writer_thread_count_;
    std::thread writer_thread(&PixieProvider::HandleAcksAndSendSubscriptionSyncsAndHeartbeats, this, std::ref(out), current_writer_number);
    writer_thread.detach();
    Log->debug("started writer thread: {}", writer_thread_count_);
}

void PixieProvider::HandleAcksAndSendSubscriptionSyncsAndHeartbeats(OutputStream& out, int thread_num)
{
    while (IsRunning() && thread_num == writer_thread_count_)
    {
        try
        {
            std::unique_ptr<AckData> ack_data = PollNextAck();

            if (ack_data != nullptr)
            {
                AckMessage message = ack_data->ToAckMessage();
                WriteMessage(message, out);
            }
            else
            {
                TryAndWriteHeartbeat(out);
            }

            PeriodicallyCheckSubscriptions(out);
        }
        catch (std::ios_base::failure& e)
        {
            Log->warn("writer thread {} encountered a ios error: {}", thread_num, e.what());
            disconnection_triggered_ = true;
            break;
        }
    }

    Log->debug("writer {} finished", thread_num);
}

void PixieProvider::PeriodicallyCheckSubscriptions(OutputStream& out)
{
    std::chrono::high_resolution_clock::time_point time_now = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration elapsed = time_now - last_subscription_check_time_;

    if (elapsed > subscription_interval_)
    {
        last_subscription_check_time_ = time_now;
        CheckAndSendSubscriptions(out);
    }
}

void PixieProvider::CheckAndSendSubscriptions(OutputStream& out)
{
    std::optional<SubscriptionSync> subscription_sync = subject_set_register_.NextSubscriptionSync();
    if (subscription_sync)
    {
        if (subscription_sync->IsChangedEdition() || negotiated_version_ >= PixieVersion::VERSION2)
        {
            subscription_sync->SetCompressed(true);
            Log->info("synchronising subscription with {}", subscription_sync->ToString());
            WriteMessage(*subscription_sync, out);
        }
    }
}

void PixieProvider::TryAndWriteHeartbeat(OutputStream& out)
{
    std::chrono::high_resolution_clock::time_point time_now = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration elapsed = time_now - last_write_time_;

    if (elapsed > protocol_options_.GetHeartbeatInterval())
    {
        WriteMessage(HeartbeatMessage(), out);
    }
}

std::unique_ptr<AckData> PixieProvider::PollNextAck()
{
    std::chrono::milliseconds timeout = std::chrono::milliseconds(std::max<long long int>((subscription_interval_ / 4).count(), 1L));

    auto ack_iterator = ack_queue_.Poll(std::chrono::milliseconds(timeout));

    if (ack_iterator == ack_queue_.End())
    {
        return nullptr;
    }

    return std::make_unique<AckData>(*ack_iterator);
}

void PixieProvider::WriteMessage(const PixieMessage& message, OutputStream& out)
{
    ByteBuffer buffer = message.Encode(negotiated_version_);
    Write(buffer, out);
}

void PixieProvider::Write(ByteBuffer& buffer, OutputStream& out)
{
    size_t frame_length = buffer.ReadableBytes();
    Varint::WriteU32(out, static_cast<unsigned int>(frame_length));
    unsigned char* bytes_to_write = buffer.ToArray();
    out.WriteBytes(bytes_to_write, frame_length);
    delete(bytes_to_write);
    last_write_time_ = std::chrono::high_resolution_clock::now();
}

SubjectSetRegister& PixieProvider::GetSubjectSetRegister()
{
    return subject_set_register_;
}

IDataDictionary& PixieProvider::GetDataDictionary()
{
    return *data_dictionary_;
}

void PixieProvider::OnConnectionError()
{
    writer_thread_count_ ++;
    disconnection_triggered_ = false;
    subject_set_register_.Reset();
}

provider::Provider::Status PixieProvider::GetStatus()
{
    return provider_status_;
}

PixieProvider::PriceSyncVisitor::PriceSyncVisitor(PixieProvider& pixie_provider, std::vector<Subject>& subject_set) : pixie_provider(pixie_provider), subject_set_(subject_set)
{}

void PixieProvider::PriceSyncVisitor::PriceImage(uint32_t sid, std::map<std::string, PriceField> price)
{
    if (Log->level() == spdlog::level::trace)
    {
        Log->trace("received price image SID: {}, price: {}", sid, LoggingUtils::MapToString<std::string, PriceField>(price));
    }
    HandlePriceUpdateEvent(sid, price, pixie_provider.subject_set_register_, true);
}

void PixieProvider::PriceSyncVisitor::PriceUpdate(uint32_t sid, std::map<std::string, PriceField> price)
{
    if (Log->level() == spdlog::level::trace)
    {
        Log->trace("received price update SID: {}, price: {}", sid, LoggingUtils::MapToString<std::string, PriceField>(price));
    }
    HandlePriceUpdateEvent(sid, price, pixie_provider.subject_set_register_, false);
}

void PixieProvider::PriceSyncVisitor::PriceStatus(uint32_t sid, SubscriptionStatus status, std::string explanation)
{
    if (Log->level() == spdlog::level::trace)
    {
        Log->trace("received price status SID: {}, status: {}, reason: {}", sid, status, explanation);
    }
    const Subject& subject = subject_set_.at(sid);
    if (pixie_provider.GetSubjectSetRegister().IsCurrentlySubscribed(subject))
    {
        pixie_provider.PublishSubscriptionStatus(subject, status, explanation);
    }

}

void PixieProvider::PriceSyncVisitor::HandlePriceUpdateEvent(uint32_t sid, std::map<std::string, PriceField>& price, SubjectSetRegister& subjectSetRegister,
                                                             bool replaceAllFields)
{
    Subject subject = subject_set_.at(sid);
    if (subjectSetRegister.IsCurrentlySubscribed(subject))
    {
        pixie_provider.PublishPriceUpdate(subject, price, replaceAllFields);
    }
}


} // namespace bidfx_public_api::price::pixie