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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_HEARTBEAT_MESSAGE_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_HEARTBEAT_MESSAGE_H_


#include "pixie_message.h"

namespace bidfx_public_api::price::pixie
{

/**
 * A Heartbeat is used to keep alive connections with little or no traffic.
 * @author Frazer Bennett Wilford
 */
class HeartbeatMessage : public PixieMessage
{

public:
    explicit HeartbeatMessage();

    ByteBuffer Encode(int version) const override;

    const bool operator==(const PixieMessage& other) const;
    const bool operator!=(const PixieMessage& other) const;

    const std::string ToString() override;
};

}

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_HEARTBEAT_MESSAGE_H_
