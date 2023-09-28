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
#include "include/api_session.h"
#include "src/basic_user_info.h"
#include "src/price/pricing_session.h"

namespace bidfx_public_api::impl
{

using bidfx_public_api::price::PricingSession;

ApiSession::ApiSession()
{
    pricing_ = new PricingSession((BasicUserInfo*) this);
}

ApiSession::~ApiSession()
{
    delete(pricing_);
}
Session& ApiSession::SetHost(std::string host)
{
    host_ = host;
    return *this;
}

Session& ApiSession::SetPort(int port)
{
    port_ = port;
    return *this;
}

Session& ApiSession::SetUseTunnel(bool use_tunnel)
{
    use_tunnel_ = use_tunnel;
    return *this;
}

Session& ApiSession::SetUsername(std::string username)
{
    username_ = username;
    return *this;
}

Session& ApiSession::SetPassword(std::string password)
{
    password_ = password;
    return *this;
}

Session& ApiSession::SetDefaultAccount(std::string buy_side_account)
{
    default_account_ = buy_side_account;
    return *this;
}

Session& ApiSession::SetApplication(std::string application)
{
    application_ = application;
    return *this;
}

Session& ApiSession::SetApplicationVersion(std::string application_version){
    application_version_ = application_version;
    return *this;
}

Session& ApiSession::SetProductSerialNumber(std::string product_serial_number)
{
    product_serial_number_ = product_serial_number;
    return *this;
}

std::string ApiSession::GetHost()
{
    return host_;
}

std::string ApiSession::GetUsername()
{
    return username_;
}

std::string ApiSession::GetPassword()
{
    return password_;
}

std::string ApiSession::GetDefaultAccount()
{
    return default_account_;
}

std::string ApiSession::GetApplication()
{
    return application_;
}

std::string ApiSession::GetApplicationVersion()
{
    return application_version_;
}

std::string ApiSession::GetProductSerialNumber()
{
    return product_serial_number_;
}

int ApiSession::GetPort()
{
    return port_;
}

bool ApiSession::IsTunnelRequired()
{
    return use_tunnel_;
}

Pricing& ApiSession::pricing()
{
    return *pricing_;
}


}