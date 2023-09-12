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

#ifndef PUBLIC_API_CPP_PRICE_SESSION_H_
#define PUBLIC_API_CPP_PRICE_SESSION_H_

#include <functional>
#include <include/pricing.h>
#include <include/user_info.h>
#include "price_update_event.h"
#include "provider_status_event.h"
#include "subscription_status_event.h"

namespace bidfx_public_api
{

/**
 * This interface provides a session to BidFX over which services may be requested.
 * @author Frazer Bennett Wilford
 */
class Session : public UserInfo
{
public:
    /**
     * Sets the host used to connect to BidFX service via.
     */
    virtual Session& SetHost(std::string host) = 0;

    /**
     * Sets the host used to connect to BidFX service via.
     */
    virtual Session& SetPort(int port) = 0;

    /**
     * Sets the username used to log into BidFX for pricing and trading.
     */
    virtual Session& SetUsername(std::string username) = 0;

    /**
     * Sets the default buy side account use for booking FX orders.
     */
    virtual Session& SetDefaultAccount(std::string buy_side_account) = 0;

    /**
     * Sets the password used to log into BidFX for pricing and trading.
     */
    virtual Session& SetPassword(std::string password) = 0;

    /**
     * Sets the name of the application being used to connect to the server. Used for client administration
     */
    virtual Session& SetApplication(std::string application) = 0;

    /**
     * Sets the version number of the application. Used for client administration
     */
    virtual Session& SetApplicationVersion(std::string api_version) = 0;

    /**
     * Sets the serial number assigned by BidFX for the product. Used for product licensing purposes.
     */
    virtual Session& SetProductSerialNumber(std::string product_serial_number) = 0;

    /**
     * Gets the session use subscribe to pricing on.
     */
    virtual Pricing& pricing() = 0;
};

} // bidfx_public_api


#endif //PUBLIC_API_CPP_SESSION_H_
