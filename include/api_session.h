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

#ifndef PUBLIC_API_CPP_API_IMPL_SESSION_H_
#define PUBLIC_API_CPP_API_IMPL_SESSION_H_

#include <include/session.h>

namespace bidfx_public_api::impl
{

using bidfx_public_api::Session;

/**
 * This class provides a session to BidFX over which services may be requested. Most applications require only a single session but
 * the API supports multiple sessions should they be needed, perhaps to connect to multiple regions.
 * @author Frazer Bennett Wilford
 */
class ApiSession : public Session
{
private:
    std::string host_;
    int port_;
    std::string username_;
    std::string password_;
    std::string default_account_;
    std::string application_;
    std::string application_version_;
    std::string product_serial_number_;

    Pricing* pricing_;

public:
    ApiSession();
    ~ApiSession();

    Session& SetHost(std::string host) override;
    Session& SetPort(int port) override;
    Session& SetUsername(std::string username) override;
    Session& SetPassword(std::string password) override;
    Session& SetDefaultAccount(std::string buy_side_account) override;

    Session& SetApplication(std::string application) override;
    Session& SetApplicationVersion(std::string application_version) override;
    Session& SetProductSerialNumber(std::string product_serial_number) override;

    std::string GetHost() override;
    std::string GetUsername() override;
    std::string GetPassword() override;
    std::string GetDefaultAccount() override;

    std::string GetApplication() override;
    std::string GetApplicationVersion() override;
    std::string GetProductSerialNumber() override;

    bool IsSSLRequired() override;
    bool IsTunnelRequired() override;

    int GetPort() override;

    Pricing& pricing() override;
};

}

#endif //PUBLIC_API_CPP_API_IMPL_SESSION_H_
