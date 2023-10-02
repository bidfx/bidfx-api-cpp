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

#ifndef PUBLIC_API_CPP_TOOLS_TUNNEL_CONNECTOR_H_
#define PUBLIC_API_CPP_TOOLS_TUNNEL_CONNECTOR_H_

#include "include/user_info.h"
#include "client.h"
#include "src/tools/byte_buffer.h"
#include "output_stream.h"
#include "input_stream.h"
#include "lib/logging/spdlog/spdlog.h"
#include "lib/logging/spdlog/logger.h"

namespace bidfx_public_api::tools
{

/**
 * This class provides a tunnel client for connecting to services via an HTTP proxy.
 * @author Frazer Bennett Wilford
 */
class TunnelConnector
{
private:
    static std::shared_ptr<spdlog::logger> Log;

    UserInfo& user_info_;
    std::string service_;

    void TunnelThroughToServer(Client& client);
    void WriteTunnelRequest(OutputStream& stream);
    void ReadTunnelResponse(InputStream& stream);
    std::string ReadLine(InputStream& stream);

public:
    TunnelConnector(UserInfo& user_info, std::string service);
    std::shared_ptr<Client> Connect(std::chrono::milliseconds read_timeout);
};


}

#endif //PUBLIC_API_CPP_TOOLS_TUNNEL_CONNECTOR_H_
