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
#include "grant_message.h"
#include "pixie_message_type.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::Varint;

GrantMessage::GrantMessage(ByteBuffer& message_frame)
{
    granted_ = 't' == message_frame.ReadByte();
    reason_ = Varint::ReadString(message_frame);
}

GrantMessage::GrantMessage(bool granted, std::string reason)
{
    granted_ = granted;
    reason_ = reason;
}

GrantMessage GrantMessage::AccessGranted()
{
    return GrantMessage(true, "");
}

GrantMessage GrantMessage::AccessDenied(std::string reason)
{
    return GrantMessage(false, reason);
}

bool GrantMessage::IsGranted() const
{
    return granted_;
}

std::string GrantMessage::GetReason() const
{
    return reason_;
}

ByteBuffer GrantMessage::Encode(int version) const
{
    size_t message_len = sizeof(PixieMessageType::GRANT) + sizeof(bool) + reason_.length() + 1;
    ByteBuffer message_frame(message_len);
    message_frame.WriteByte(PixieMessageType::GRANT);
    message_frame.WriteByte(granted_ ? 't' : 'f');
    Varint::WriteString(message_frame, reason_);

    return std::move(message_frame);
}

const std::string GrantMessage::ToString()
{
    std::stringstream ss;

    ss << "Grant(granted=";

    if (granted_)
    {
        ss << "true";
    }
    else
    {
        ss << "false";
    }
    ss << ", reason=" << reason_ << ")";
    return ss.str();
}


}