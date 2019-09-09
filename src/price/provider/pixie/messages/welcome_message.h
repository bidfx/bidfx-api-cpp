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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_WELCOME_MESSAGE_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_WELCOME_MESSAGE_H_

#include "src/tools/byte_buffer.h"
#include "src/tools/varint.h"
#include "pixie_message.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::ByteBuffer;
using bidfx_public_api::tools::Varint;

/**
 * A Welcome message is the first message sent from the server to the client. Its purpose to is complete the protocol version
 * negotiation handshake.
 * @author acozzoli
 */
class WelcomeMessage : public PixieMessage
{

private:
    unsigned int options_;
    unsigned int version_;
    int clientID_;
    int serverID_;

public:
    /**
     * Create a Welcome message with the given version number.
     * @param version number
     * @param clientID
     * @param serverID
     */
    WelcomeMessage(unsigned int version, uint32_t clientID, uint32_t serverID);

    /**
     * Creates a Welcome message by decoding a message frame. Only the members of the message are going to be decoded, the type will need
     * to be read from the decoder.
     * @param message_frame frame to decode
     */
    WelcomeMessage(ByteBuffer& message_frame);

    /**
     * Gets the  protocol options that the server can share with the client
     * @return options
     */
    unsigned int GetOptions();

    /**
     * Gets the server negotiated protocol version number.
     * @return version number
     */
    unsigned int GetVersion();

    /**
     * Gets the unique client ID.
     */
    unsigned int GetClientID();

    /**
     * Gets the unique server ID.
     */
    unsigned int GetServerID();

    /**
     * Gets string representation of the welcome message
     */
    const std::string ToString() override;

    ByteBuffer Encode(int version) const override;

    const bool operator==(const WelcomeMessage& other) const;

    const bool operator!=(const WelcomeMessage& other) const;
};

}


#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_WELCOME_MESSAGE_H_
