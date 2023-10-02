/**  Copyright 2020 BidFX

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

#include <ios>
#include <iostream>
#include <sstream>
#include <memory>
#include "src/tools/guid.h"
#include "include/tools/base64.h"
#include "include/tools/logger_factory.h"
#include "tunnel_connector.h"
#include "open_ssl_client.h"
#include "direct_client.h"

namespace bidfx_public_api::tools
{

using bidfx_public_api::tools::LoggerFactory;

std::shared_ptr<spdlog::logger> TunnelConnector::Log = nullptr;

TunnelConnector::TunnelConnector(UserInfo& user_info, std::string service) : user_info_(user_info), service_(service)
{
    if (Log == nullptr)
    {
        Log = LoggerFactory::GetLogger("TunnelConnector");
    }
}

std::shared_ptr<Client> TunnelConnector::Connect(std::chrono::milliseconds read_timeout)
{
    try
    {
        Log->info("Connecting to {}:{} with read timeout {}ms", user_info_.GetHost(), user_info_.GetPort(), read_timeout.count());
        std::shared_ptr<Client> client = nullptr;

        if (user_info_.IsTunnelRequired())
        {
            client = std::make_shared<OpenSSLClient>(user_info_.GetHost(), user_info_.GetPort(), read_timeout);
            TunnelThroughToServer(*client);
        }
        else
        {
            client = std::make_shared<DirectClient>(user_info_.GetHost(), user_info_.GetPort());
        }

        return std::move(client);
    }
    catch (std::ios_base::failure &e)
    {
        Log->warn("exception during handshake: {}", e.what());
        throw std::ios_base::failure(e);
    }
}

void TunnelConnector::TunnelThroughToServer(Client& client)
{
    WriteTunnelRequest(client.GetOutputStream());
    ReadTunnelResponse(client.GetInputStream());
}

void TunnelConnector::WriteTunnelRequest(OutputStream& stream)
{
    std::string credentials = user_info_.GetUsername() + ":" + user_info_.GetPassword();
    std::string base64 = bidfx_public_api::tools::Base64Encode(credentials.c_str(), credentials.length());
    std::string header = "CONNECT " + service_ + " HTTP/1.1\r\nAuthorization: Basic " + base64 + "\r\n" + "GUID: " + GUID().ToString() + "\r\n\r\n";

    stream.WriteString(header);
}

void TunnelConnector::ReadTunnelResponse(InputStream& stream) {
    std::string line = ReadLine(stream);
    bool success = line == "HTTP/1.1 200 OK";

    std::stringstream ss;

    while (!line.empty())
    {
        ss << '\n' << line;
        line = ReadLine(stream);
    }

    if (!success)
    {
        std::stringstream error_message;
        error_message << "Failed to tunnel to service " << service_ << " with response: " << ss.str();
        throw std::ios_base::failure(error_message.str());
    }

    // SUCCESS
    Log->info("tunnel connection established");
}

std::string TunnelConnector::ReadLine(InputStream& stream)
{
    char b = stream.ReadByte();

    std::stringstream ss;

    while (b != '\r')
    {
        char c = b;
        ss << c;
        b = stream.ReadByte();
    }

    b = stream.ReadByte();

    std::string response = ss.str();

    if (b != '\n')
    {
        std::stringstream error_message;
        error_message << "Expected line feed char at end of HTTP reponse (tunnel connector): " << response;
        throw std::ios_base::failure(error_message.str());
    }

    return response;
}


}