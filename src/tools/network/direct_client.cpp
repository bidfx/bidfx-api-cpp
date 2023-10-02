/**  Copyright 2023 BidFX

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

#include <unistd.h>
#include <malloc.h>
#include <arpa/inet.h>

#include <utility>
#include "include/tools/logger_factory.h"
#include "tunnel_connector.h"
#include "direct_client.h"

#define FAIL    -1

namespace bidfx_public_api::tools
{
using bidfx_public_api::tools::LoggerFactory;
std::shared_ptr<spdlog::logger> DirectClient::Log = nullptr;

DirectClient::DirectClient(std::string host, int port) : output_stream_(OutputStreamImpl(*this)), input_stream_(InputStreamImpl(*this))
{
    if (Log == nullptr)
    {
        Log = LoggerFactory::GetLogger("DirectClient");
    }
    host_ = std::move(host);
    port_ = port;
}

void DirectClient::Start()
{
    server_ = OpenConnection(host_, port_);

    if (Log->level() >= SPDLOG_LEVEL_DEBUG)
    {
        Log->debug("Connected via socket");
    }
}

void DirectClient::Close()
{
    Log->trace("closing socket");
    close(server_);
}

OutputStream& DirectClient::GetOutputStream()
{
    return output_stream_;
}

InputStream& DirectClient::GetInputStream()
{
    return input_stream_;
}

/************************
* Output Stream
***********************/
DirectClient::OutputStreamImpl::OutputStreamImpl(DirectClient& direct_client)
{
    direct_client_ = &direct_client;
}

size_t DirectClient::OutputStreamImpl::WriteByte(const unsigned char c)
{
    return WriteBytes(&c, 1);
}

size_t DirectClient::OutputStreamImpl::WriteBytes(const unsigned char* c, size_t len)
{
    send(direct_client_->server_, c, len, 0);
    return len;
}

size_t DirectClient::OutputStreamImpl::WriteString(const std::string& str)
{
    size_t len = str.size();
    return WriteBytes(reinterpret_cast<const unsigned char*>(str.data()), len);
}

/************************
* Input Stream
***********************/
DirectClient::InputStreamImpl::InputStreamImpl(DirectClient& direct_client)
{
    direct_client_ = &direct_client;
}

unsigned char DirectClient::InputStreamImpl::ReadByte()
{
    auto* bytes = new unsigned char[1];
    size_t bytes_read = ReadBytes(bytes, 1);
    if (bytes_read == -1)
    {
        throw std::ios_base::failure("Could not read byte");
    }

    unsigned char byte = bytes[0];
    delete[](bytes);
    return byte;
}

size_t DirectClient::InputStreamImpl::ReadBytes(unsigned char *buf, size_t len)
{
    size_t bytes = read(direct_client_->server_, buf, len);
    if (bytes == -1)
    {
        Log->info("Could not read bytes");
    }
    return bytes;
}

}
