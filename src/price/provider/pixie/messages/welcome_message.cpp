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

#include <sstream>
#include "src/tools/varint.h"
#include "welcome_message.h"
#include "pixie_message_type.h"

namespace bidfx_public_api::price::pixie
{

WelcomeMessage::WelcomeMessage(unsigned int version, uint32_t clientID, uint32_t serverID)
{
    version_ = version;
    clientID_ = clientID;
    serverID_ = serverID;
}

WelcomeMessage::WelcomeMessage(ByteBuffer& message_frame)
{
    options_ = Varint::ReadU32(message_frame);
    version_ = Varint::ReadU32(message_frame);
    clientID_ = message_frame.ReadInt();
    serverID_ = message_frame.ReadInt();
}

unsigned int WelcomeMessage::GetOptions()
{
    return options_;
}

unsigned int WelcomeMessage::GetVersion()
{
    return version_;
}

unsigned int WelcomeMessage::GetClientID()
{
    return clientID_;
}

unsigned int WelcomeMessage::GetServerID()
{
    return serverID_;
}

const bool WelcomeMessage::operator==(const WelcomeMessage& other) const
{
    return clientID_ == other.clientID_ &&
        options_ == other.options_ &&
        version_ == other.version_ &&
        serverID_ == other.serverID_;

}

const bool WelcomeMessage::operator!=(const WelcomeMessage& other) const
{
    return !(*this == other);
}

const std::string WelcomeMessage::ToString()
{
    std::stringstream ss;
    ss << "Welcome(version=" << version_ << ", clientID=0x" << std::hex << clientID_ << ", serverID=0x" << serverID_ << ")";
    return ss.str();
}

ByteBuffer WelcomeMessage::Encode(int version) const
{
    size_t message_len = sizeof(PixieMessageType::WELCOME) + sizeof(options_) +
                         sizeof(version_) + sizeof(clientID_) + sizeof(serverID_);

    ByteBuffer message_frame(message_len);
    message_frame.WriteByte(PixieMessageType::WELCOME);
    Varint::WriteU32(message_frame, options_);
    Varint::WriteU32(message_frame, version_);
    message_frame.WriteInt(clientID_);
    message_frame.WriteInt(serverID_);

    return std::move(message_frame);
}
}
