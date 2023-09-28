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

#ifndef BIDFX_PUBLIC_API_CPP_DIRECT_CLIENT_H
#define BIDFX_PUBLIC_API_CPP_DIRECT_CLIENT_H

#include <chrono>
#include "lib/logging/spdlog/logger.h"
#include "client.h"

namespace bidfx_public_api::tools
{

class DirectClient : public Client {
private:
    static std::shared_ptr<spdlog::logger> Log;

    class OutputStreamImpl : public OutputStream
    {
    private:
        DirectClient* direct_client_;

    public:
        explicit OutputStreamImpl(DirectClient& direct_client);

        size_t WriteByte(unsigned char c) override;
        size_t WriteBytes(const unsigned char* c, size_t len) override;
        size_t WriteString(const std::string&) override;
    };

    class InputStreamImpl : public InputStream
    {
    private:
        DirectClient* direct_client_;

    public:
        explicit InputStreamImpl(DirectClient& direct_client);

        unsigned char ReadByte() override;
        size_t ReadBytes(unsigned char* buf, size_t len) override;
    };

    std::string host_;
    int port_;
    int server_;

    OutputStreamImpl output_stream_;
    InputStreamImpl input_stream_;
public:
    DirectClient(std::string host, int port);

    ~DirectClient() override = default;

    void Start() override;

    void Close() override;

    OutputStream& GetOutputStream() override;

    InputStream& GetInputStream() override;

};

}

#endif //BIDFX_PUBLIC_API_CPP_DIRECT_CLIENT_H
