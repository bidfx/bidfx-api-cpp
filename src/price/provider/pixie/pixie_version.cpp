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

#include <iostream>
#include <sstream>
#include "include/tools/logger_factory.h"
#include "pixie_version.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::LoggerFactory;

const int PixieVersion::VERSION1 = 1;
const int PixieVersion::VERSION2 = 2;
const int PixieVersion::VERSION3 = 3;
const int PixieVersion::VERSION4 = 4;
const int PixieVersion::CURRENT_VERSION = PixieVersion::VERSION4;

std::shared_ptr<spdlog::logger> PixieVersion::Log = nullptr;

int PixieVersion::ServerNegotiateVersion(int client_version)
{
    EnsureLoggerIsActive();
    int server_negotiate_version = ServerNegotiateVersion(client_version, VERSION1, CURRENT_VERSION);
    Log->debug("client proposed pixie version {}, server negotiated version {}", client_version, server_negotiate_version);
    return server_negotiate_version;
}

int PixieVersion::ServerNegotiateVersion(int client_version, int server_from, int server_to)
{
    if (client_version < server_from || client_version > server_to) return server_to;
    return client_version;
}

void PixieVersion::ClientVerifyVersion(int server_version)
{
    ClientVerifyVersion(server_version, VERSION1, CURRENT_VERSION);
}

void PixieVersion::ClientVerifyVersion(int server_version, int client_from, int client_to)
{
    EnsureLoggerIsActive();

    if (server_version < client_from || server_version > client_to)
    {
        std::stringstream ss;
        ss << "a server negotiating version " << server_version << " is incompatible with client on pixie version " << client_from;
        if (client_to > client_from)
        {
            ss << " to " << client_to;
        }

        ss << std::endl;
        std::string error_message = ss.str();

        Log->warn(error_message);
        throw std::invalid_argument(error_message);
    }

    Log->debug("client and server have agreed on pixie Version {}", server_version);
}

void PixieVersion::EnsureLoggerIsActive()
{
    if (Log == nullptr)
    {
        Log = LoggerFactory::GetLogger("PixieVersion");
    }
}


}