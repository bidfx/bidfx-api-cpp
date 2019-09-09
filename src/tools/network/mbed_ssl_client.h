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


#ifndef BIDFX_PUBLIC_API_MBED_SSL_CLIENT_H_
#define BIDFX_PUBLIC_API_MBED_SSL_CLIENT_H_

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <ctdio.h>
#include <stdlib.h>
#define mbedtls_time       time
#define mbedtls_time_t     time_t
#define mbedtls_fprintf    fprintf
#define mbedtls_printf     printf
#endif

#if !defined(MBEDTLS_BIGNUM_C) || !defined(MBEDTLS_ENTROPY_C) ||  \
    !defined(MBEDTLS_SSL_TLS_C) || !defined(MBEDTLS_SSL_CLI_C) || \
    !defined(MBEDTLS_NET_C) || !defined(MBEDTLS_RSA_C) ||         \
    !defined(MBEDTLS_CERTS_C) || !defined(MBEDTLS_PEM_PARSE_C) || \
    !defined(MBEDTLS_CTR_DRBG_C) || !defined(MBEDTLS_X509_CRT_PARSE_C)
int main( void )
{
    mbedtls_printf("MBEDTLS_BIGNUM_C and/or MBEDTLS_ENTROPY_C and/or "
           "MBEDTLS_SSL_TLS_C and/or MBEDTLS_SSL_CLI_C and/or "
           "MBEDTLS_NET_C and/or MBEDTLS_RSA_C and/or "
           "MBEDTLS_CTR_DRBG_C and/or MBEDTLS_X509_CRT_PARSE_C "
           "not defined.\n");
    return( 0 );
}
#else

#include "mbedtls/net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"

#include "src/tools/byte_buffer.h"
#include "ssl_client.h"
#include "output_stream.h"

#include <string>

namespace bidfx_public_api::tools
{

class MBedSSLClient : public SSLClient {
private:
    class OutputStreamImpl : public OutputStream
    {
    private:
        MBedSSLClient* m_bed_client_;

    public:
        explicit OutputStreamImpl(MBedSSLClient& m_bed_client);

        size_t WriteByte(const unsigned char c) override;
        size_t WriteBytes(const unsigned char* c, size_t len) override;
        size_t WriteString(const std::string&) override;
    };

    class InputStreamImpl : public InputStream
    {
    private:
        MBedSSLClient* m_bed_client_;

    public:
        explicit InputStreamImpl(MBedSSLClient& m_bed_client);

        unsigned char ReadByte() override;
        size_t ReadBytes(unsigned char* buf, size_t len) override;
    };

    const char* pers = "ssl_client1";
    std::string host_;
    int port_;

    mbedtls_net_context server_fd_;
    uint32_t flags_;

    mbedtls_entropy_context entropy_;
    mbedtls_ctr_drbg_context ctr_drbg_;
    mbedtls_ssl_context ssl_;
    mbedtls_ssl_config conf_;
    mbedtls_x509_crt cacert_;

    OutputStreamImpl output_stream_;
    InputStreamImpl input_stream_;

    void InitializeSessionData();
    void InitializeCertificates();
    void StartConnection();
    void SetupConfigDefaults();
    void Handshake();
    void VerifyCertificates();
    char* GetErrorMessage(int value);

public:
    MBedSSLClient(std::string host, int port);
    void Start() override;
    void Close() override;

    OutputStream& GetOutputStream() override;
    InputStream& GetInputStream() override;
};

}


#endif /* MBEDTLS_BIGNUM_C && MBEDTLS_ENTROPY_C && MBEDTLS_SSL_TLS_C &&
          MBEDTLS_SSL_CLI_C && MBEDTLS_NET_C && MBEDTLS_RSA_C &&
          MBEDTLS_CERTS_C && MBEDTLS_PEM_PARSE_C && MBEDTLS_CTR_DRBG_C &&
          MBEDTLS_X509_CRT_PARSE_C */

#endif //BIDFX_PUBLIC_API_MBED_SSL_CLIENT_H_
