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

#ifndef PUBLIC_API_CPP_TOOLS_OPEN_SSL_CLIENT_H_
#define PUBLIC_API_CPP_TOOLS_OPEN_SSL_CLIENT_H_

#include <chrono>
#include <openssl/ssl.h>
#include "lib/logging/spdlog/logger.h"
#include "ssl_client.h"

namespace bidfx_public_api::tools
{

class OpenSSLClient : public SSLClient {
private:
    static std::shared_ptr<spdlog::logger> Log;

    class OutputStreamImpl : public OutputStream
    {
    private:
        OpenSSLClient* open_ssl_client_;

    public:
        explicit OutputStreamImpl(OpenSSLClient& open_ssl_client);

        size_t WriteByte(const unsigned char c) override;
        size_t WriteBytes(const unsigned char* c, size_t len) override;
        size_t WriteString(const std::string&) override;
    };

    class InputStreamImpl : public InputStream
    {
    private:
        OpenSSLClient* open_ssl_client_;

    public:
        explicit InputStreamImpl(OpenSSLClient& open_ssl_client);

        unsigned char ReadByte() override;
        size_t ReadBytes(unsigned char* buf, size_t len) override;
    };

    SSL_CTX *ctx_;
    int server_;
    SSL *ssl_;
    std::string host_;
    int port_;
    std::chrono::milliseconds read_timeout_;

    OutputStreamImpl output_stream_;
    InputStreamImpl input_stream_;

    int OpenConnection(const std::string& host, int port);
    SSL_CTX* InitCTX();
    void ShowCerts(SSL* ssl);
    std::string OpenSSLErrAsString();
public:
    OpenSSLClient(std::string host, int port, std::chrono::milliseconds read_timeout);

    ~OpenSSLClient() override = default;

    void Start() override;

    void Close() override;

    OutputStream& GetOutputStream() override;

    InputStream& GetInputStream() override;

};

}

#endif //PUBLIC_API_CPP_TOOLS_OPEN_SSL_CLIENT_H_
