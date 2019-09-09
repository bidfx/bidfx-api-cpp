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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_GRANT_MESSAGE_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_GRANT_MESSAGE_H_

#include "src/tools/byte_buffer.h"
#include "pixie_message.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::ByteBuffer;

/**
 * An Grant message is used once by a server to respond to a Login message.
 * @author Frazer Bennett Wilford
 */
class GrantMessage : public PixieMessage
{
private:
    bool granted_;
    std::string reason_;

public:
    /**
     * Creates an instance by decoding the data from a buffer.
     * @param message_frame to decode
     */
    GrantMessage(ByteBuffer& message_frame);

    GrantMessage(bool granted, std::string reason);

    /**
     * Creates an access granted message.
     */
    static GrantMessage AccessGranted();

    /**
     * Creates an access denied message.
     * @param reason the reason for the denial.
     */
    static GrantMessage AccessDenied(std::string reason);

    /**
     * Checks if access was granted.
     */
    bool IsGranted() const;

    /**
     * Gets the message giving the reason for access being refused.
     */
    std::string GetReason() const;

    ByteBuffer Encode(int version) const override;
    const std::string ToString() override;
};

}

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_GRANT_MESSAGE_H_
