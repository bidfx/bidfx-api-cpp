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

#include <stdexcept>
#include "src/tools/byte_buffer.h"
#include "ack_message.h"
#include "src/price/provider/pixie/messages/pixie_message_type.h"
#include "src/tools/varint.h"
#include "include/tools/java_time.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::Varint;
using bidfx_public_api::tools::JavaTime;

AckMessage::AckMessage(long revision, long revision_time, long price_received_time, long ack_time,
                       long handling_duration_micros)
{
    this->revision_ = revision;
    this->revision_time_ = revision_time;
    this->price_received_time_ = price_received_time;
    this->ack_time_ = ack_time;

    if (handling_duration_micros >= 0)
    {
        handling_duration_ = handling_duration_micros;
    } else
    {
        throw std::invalid_argument("IllegalArgumentException: handling_duration_micros must be positive");
    }
}

ByteBuffer AckMessage::Encode(int version) const
{
    size_t num_bytes = 33;
    if (version >= 2)
    {
        num_bytes += 8;
    }

    ByteBuffer byte_buffer(num_bytes);
    byte_buffer.WriteByte(PixieMessageType::ACK);

    Varint::WriteU64(byte_buffer, revision_);
    Varint::WriteU64(byte_buffer, revision_time_);
    Varint::WriteU64(byte_buffer, price_received_time_);
    Varint::WriteU64(byte_buffer, ack_time_);

    if (version >= 2)
    {
        Varint::WriteU64(byte_buffer, handling_duration_);
    }

    return std::move(byte_buffer);
}

const std::string AckMessage::ToString()
{
    return std::string("Ack(revision=" + std::to_string(revision_) + ", revisionTime=" +
                       std::to_string(JavaTime::ToIsoTime(revision_time_))
                       + ", priceReceivedTime=" + std::to_string(JavaTime::ToIsoTime(price_received_time_))
                       + ", ackTime=" + std::to_string(JavaTime::ToIsoTime(ack_time_))
                       + ", handlingDuration=" + std::to_string(handling_duration_)
                       + "us)"
    );
}

const bool AckMessage::operator==(const AckMessage& other) const
{
    return revision_ == other.revision_ && revision_time_ == other.revision_time_ &&
           price_received_time_ == other.price_received_time_ && ack_time_ == other.ack_time_ &&
           handling_duration_ == other.handling_duration_;
}

const bool AckMessage::operator!=(const AckMessage &other) const
{
    return !(*this == other);
}

long AckMessage::GetRevision() const
{
    return revision_;
}

long AckMessage::GetRevisionTime() const
{
    return revision_time_;
}

long AckMessage::GetPriceReceivedTime() const
{
    return price_received_time_;
}

long AckMessage::GetAckTime() const
{
    return ack_time_;
}

long AckMessage::GethandlingDurationMicros() const
{
    return handling_duration_;
}

}// namespace bidfx_public_api::price::pixie