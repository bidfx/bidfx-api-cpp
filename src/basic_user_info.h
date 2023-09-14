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

#ifndef PUBLIC_API_CPP_BASIC_USER_INFO_H_
#define PUBLIC_API_CPP_BASIC_USER_INFO_H_

#include "include/user_info.h"

namespace bidfx_public_api
{

class BasicUserInfo : public UserInfo
{
private:
    std::string host_;
    int         port_;
    bool        use_tunnel_;
    std::string username_;
    std::string password_;
    std::string default_account_;
    std::string application_;
    std::string application_version_;
    std::string product_serial_number_;

public:
    BasicUserInfo(std::string host, std::string username, std::string default_account, std::string application,
                  std::string application_version, std::string product_serial_number);

    BasicUserInfo(std::string host, int port, bool use_tunenl, std::string username, std::string default_account,
                  std::string application, std::string application_version, std::string product_serial_number);

    std::string GetHost() override;
    std::string GetUsername() override;
    std::string GetPassword() override;
    std::string GetDefaultAccount() override;
    std::string GetApplication() override;
    std::string GetApplicationVersion() override;
    std::string GetProductSerialNumber() override;
    int GetPort() override;
    bool IsSSLRequired() override;
    bool IsTunnelRequired() override;
};

}

#endif //PUBLIC_API_CPP_BASIC_USER_INFO_H_
