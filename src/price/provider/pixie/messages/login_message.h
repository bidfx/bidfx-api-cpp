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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_LOGIN_MESSAGE_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_LOGIN_MESSAGE_H_

#include <string>
#include "src/tools/varint.h"
#include "pixie_message.h"
#include "welcome_message.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::Varint;

/**
 * A Login message is used once by a client to log in to the server. It provides the client's credentials. These are usually
 * verified by the server using Sesame.
 * @author Frazer Bennett Wilford
 */
class LoginMessage : public PixieMessage
{
private:
    std::string username_;
    std::string password_;
    std::string alias_;
    std::string application_;
    std::string application_version_;
    std::string api_;
    std::string api_version_;
    std::string product_;
    std::string product_serial_number_;

public:
    /**
     * Create a Login message with the given credentials.
     * @param username the username.
     * @param password the user's password.
     * @param alias the users OS login name.
     * @param application the name of the application using the client API (for admin purposes).
     * @param application_version the version of the application using the client API (for admin purposes).
     * @param api the name of the api being used to connect to the server. Used for client administration
     * @param api_version the version number of the connecting application. Used for client administration.
     * @param product_serial_number serial number assigned by BidFX for the product. Used for product licensing purposes.
     */
    LoginMessage(std::string username, std::string password, std::string alias, std::string application,
                 std::string application_version, std::string api, std::string api_version, std::string product_serial_number);

    /**
     * Creates a Login message by decoding a message frame. Only the members of the message are going to be decoded, the type will need to
     * be read from the decoder.
     * @param message_frame to decode
     * @param version pixie version that has been negotiated with server
     */
    explicit LoginMessage(ByteBuffer& message_frame, int version);

    ByteBuffer Encode(int version) const override;

    /**
     * Gets the user's server login name.
     */
    std::string GetUsername();

    /**
     * Gets the user's server login password.
     */
    std::string GetPassword();

    /**
     * Gets the user's alias name - normally their OS login name.
     */
    std::string GetAlias();

    /**
     * Gets the name of the application using the client API (for admin purposes).
     * @since protocol version 2
     */
    std::string GetApplication();

    /**
     * Gets the version of the application using the client API (for admin purposes).
     * @since protocol version 2
     */
    std::string GetApplicationVersion();

    /**
     * Gets the name of the api being used to connect to the server. Used for client administration
     * @since protocol version 4
     */
    std::string GetApi();

    /**
     * Gets the version number of the connecting application. Used for client administration.
     * @since protocol version 4
     */
    std::string GetApiVersion();

    /**
     * Gets the version number of the connecting application. Used for client administration.
     * @since protocol version 4
     */
    std::string GetProduct();

    /**
     * Gets the version number of the connecting application. Used for client administration.
     * @since protocol version 4
     */
    std::string GetProductSerialNumber();

    const bool operator==(const LoginMessage& other) const;

    const bool operator!=(const LoginMessage& other) const;

    const std::string ToString() override;

};

} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_LOGIN_MESSAGE_H_
