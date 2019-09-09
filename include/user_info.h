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

#ifndef PUBLIC_API_CPP_USER_INFO_H_
#define PUBLIC_API_CPP_USER_INFO_H_

#include <string>

namespace bidfx_public_api {

/**
 * This interface provides access to user information associated with a login session.
 * @author Frazer Bennett Wilford
 */
class UserInfo {

public:
    /**
     * Gets the host used to connect to BidFX service via.
     */
    virtual std::string GetHost() = 0;

    /**
     * Gets the username used to log into BidFX for pricing and trading.
     */
    virtual std::string GetUsername() = 0;

    /**
     * Gets the password used to log into BidFX for pricing and trading.
     */
    virtual std::string GetPassword() = 0;

    /**
     * Gets the default buy side account use for booking OTC orders.
     */
    virtual std::string GetDefaultAccount() = 0;

    /**
     * Gets the name of the api being used to connect to the server. Used for client administration
     */
    virtual std::string GetApplication() = 0;

    /**
     * Gets the version number of the api. Used for client administration
     */
    virtual std::string GetApplicationVersion() = 0;

    /**
     * Gets the serial number assigned by BidFX for the product. Used for product licensing purposes.
     */
    virtual std::string GetProductSerialNumber() = 0;

    /**
     * Gets the port number used to connect to BidFX service via.
     */
    virtual int GetPort() = 0;

    virtual bool IsSSLRequired() = 0;

    virtual bool IsTunnelRequired() = 0;
};

}

#endif //PUBLIC_API_CPP_USER_INFO_H_
