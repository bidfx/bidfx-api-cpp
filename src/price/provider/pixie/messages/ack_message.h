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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_ACK_MESSAGE_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_ACK_MESSAGE_H_

#include <string>
#include <memory>
#include <functional>
#include "pixie_message.h"

namespace bidfx_public_api::price::pixie
{

class AckMessage : public PixieMessage
{
private:
    uint64_t revision_;
    uint64_t revision_time_;
    uint64_t price_received_time_;
    uint64_t ack_time_;
    uint64_t handling_duration_; // since version 2

public:
    AckMessage(long revision, long revision_time, long price_received_time, long ack_time,
               long handling_duration_micros);

    long GetRevision() const;

    long GetRevisionTime() const;

    long GetPriceReceivedTime() const;

    long GetAckTime() const;

    long GethandlingDurationMicros() const;

    ByteBuffer Encode(int version)const ;

    const std::string ToString();

    const bool operator==(const AckMessage& other) const;

    const bool operator!=(const AckMessage& other) const;
};

} // namespace bidfx_public_api::price::pixie

namespace std {
using bidfx_public_api::price::pixie::AckMessage;

template<>
struct hash<AckMessage>
{
    std::size_t operator()(const AckMessage &message) const {
        return std::hash<long>()(message.GetRevision()) ^
               std::hash<long>()(message.GetAckTime()) ^
               std::hash<long>()(message.GethandlingDurationMicros()) ^
               std::hash<long>()(message.GetPriceReceivedTime()) ^
               std::hash<long>()(message.GetRevisionTime());
    }
};
}

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_ACK_MESSAGE_H_
