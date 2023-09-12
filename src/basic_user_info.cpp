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

#include <string>
#include "basic_user_info.h"

namespace bidfx_public_api
{

BasicUserInfo::BasicUserInfo(std::string host, int port, std::string username, std::string default_account, std::string application,
                             std::string application_version, std::string product_serial_number)
{
    host_ = host;
    port_ = port;
    username_ = username;
    default_account_ = default_account;
    application_ = application;
    application_version_ = application_version;
    product_serial_number_ = product_serial_number;
}

    BasicUserInfo::BasicUserInfo(std::string host, std::string username, std::string default_account, std::string application,
                                 std::string application_version, std::string product_serial_number)
    {
        host_ = host;
        port_ = 443;
        username_ = username;
        default_account_ = default_account;
        application_ = application;
        application_version_ = application_version;
        product_serial_number_ = product_serial_number;
    }

std::string BasicUserInfo::GetHost()
{
    return host_;
}

std::string BasicUserInfo::GetUsername()
{
    return username_;
}

std::string BasicUserInfo::GetPassword()
{
    return "secret_chicken";
}

std::string BasicUserInfo::GetDefaultAccount()
{
    return default_account_;
}

std::string BasicUserInfo::GetApplication()
{
    return application_;
}

std::string BasicUserInfo::GetApplicationVersion()
{
    return application_version_;
}

std::string BasicUserInfo::GetProductSerialNumber()
{
    return product_serial_number_;
}

int BasicUserInfo::GetPort()
{
    return port_;
}

bool BasicUserInfo::IsSSLRequired()
{
    return GetPort() == 443 || GetPort() == 8443;
}

bool BasicUserInfo::IsTunnelRequired()
{
    return IsSSLRequired();
}

}