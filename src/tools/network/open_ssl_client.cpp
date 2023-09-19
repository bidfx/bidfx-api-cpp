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

#include <cstdio>
#include <unistd.h>
#include <malloc.h>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <utility>
#include "open_ssl_client.h"
#include "include/tools/logger_factory.h"
#include "tunnel_connector.h"

#define FAIL    -1

namespace  bidfx_public_api::tools
{
using bidfx_public_api::tools::LoggerFactory;
std::shared_ptr<spdlog::logger> OpenSSLClient::Log = nullptr;

OpenSSLClient::OpenSSLClient(std::string host, int port, std::chrono::milliseconds read_timeout) : output_stream_(OutputStreamImpl(*this)), input_stream_(InputStreamImpl(*this))
{
    if (Log == nullptr)
    {
        Log = LoggerFactory::GetLogger("OpenSSLClient");
    }
    SSL_library_init();
    host_ = std::move(host);
    port_ = port;
    read_timeout_ = read_timeout;
    ctx_ = InitCTX();
}

void OpenSSLClient::Start()
{
    SSL_CTX_set_timeout(ctx_, std::chrono::duration_cast<std::chrono::seconds>(read_timeout_).count());
    server_ = OpenConnection(host_, port_);
    ssl_ = SSL_new(ctx_);      /* create new SSL connection state */
    SSL_set_fd(ssl_, server_);    /* attach the socket descriptor */
    if ( SSL_connect(ssl_) == FAIL )   /* perform the connection */
    {
        std::string error = OpenSSLErrAsString();
        Log->warn(error);
        throw std::ios_base::failure("Could not connect to server using SSL: " + error);
    }
    if (Log->level() >= SPDLOG_LEVEL_DEBUG)
    {
        Log->debug("Connected with {} encryption", SSL_get_cipher(ssl_));
    }
    ShowCerts(ssl_);        /* get any certs */
}

void OpenSSLClient::Close()
{
    Log->trace("Closing SSL client");
    SSL_free(ssl_);
    close(server_);
}

OutputStream& OpenSSLClient::GetOutputStream()
{
    return output_stream_;
}

InputStream& OpenSSLClient::GetInputStream()
{
    return input_stream_;
}

int OpenSSLClient::OpenConnection(const std::string& hostname, int port)
{
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(hostname.c_str())) == NULL )
    {
        throw std::ios_base::failure("Could not determine host from: " + hostname);
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    int connect_result = connect(sd, (struct sockaddr*)&addr, sizeof(addr));
    if ( connect_result != 0 )
    {
        close(sd);
        throw std::ios_base::failure("Could not connect to " + hostname + ":" + std::to_string(port) + " - error code " + std::to_string(connect_result));
    }
    return sd;
}

SSL_CTX* OpenSSLClient::InitCTX()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    /* Create new client-method instance */
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    method = TLS_client_method();
#elif OPENSSL_VERSION_NUMBER >= 0x10002000L && !defined OPENSSL_NO_TLS1_2_METHOD
    method = TLSv1_2_client_method();
#else
#error "OpenSSL version must suppport TLS_v1_2"
#endif

    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == nullptr )
    {
        std::string error = OpenSSLErrAsString();
        throw std::ios_base::failure("Could not initialize SSL context: " + error);
    }
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
#endif
    return ctx;
}

void OpenSSLClient::ShowCerts(SSL* ssl)
{
    X509 *cert;
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != nullptr )
    {
        char *line;
        line = X509_NAME_oneline(X509_get_subject_name(cert), nullptr, 0);
        std::string subject = std::string(line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), nullptr, 0);
        std::string issuer = std::string(line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
        Log->info("Server certificates:\nSubject: {}\nIssuer: {}", subject, issuer);
    }
    else
    {
        Log->info("No client certificates configured.\n");
    }
}

std::string OpenSSLClient::OpenSSLErrAsString ()
{
    BIO *bio = BIO_new (BIO_s_mem ());
    ERR_print_errors (bio);
    char *buf = nullptr;
    size_t len = BIO_get_mem_data (bio, &buf);
    BIO_free (bio);
    return std::string(buf, len);
}

/************************
 * Output Stream
 ***********************/
OpenSSLClient::OutputStreamImpl::OutputStreamImpl(OpenSSLClient& open_ssl_client)
{
    open_ssl_client_ = &open_ssl_client;
}

size_t OpenSSLClient::OutputStreamImpl::WriteByte(const unsigned char c)
{
    return WriteBytes(&c, 1);
}

size_t OpenSSLClient::OutputStreamImpl::WriteBytes(const unsigned char* c, size_t len)
{
    SSL_write(open_ssl_client_->ssl_, c, len);
    return len;
}

size_t OpenSSLClient::OutputStreamImpl::WriteString(const std::string& str)
{
    size_t len = str.size();
    return WriteBytes(reinterpret_cast<const unsigned char*>(str.data()), len);
}

/************************
 * Input Stream
 ***********************/
OpenSSLClient::InputStreamImpl::InputStreamImpl(OpenSSLClient& open_ssl_client)
{
    open_ssl_client_ = &open_ssl_client;
}

unsigned char OpenSSLClient::InputStreamImpl::ReadByte()
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

size_t OpenSSLClient::InputStreamImpl::ReadBytes(unsigned char *buf, size_t len)
{
    size_t bytes = SSL_read(open_ssl_client_->ssl_, buf, len);
    if (bytes == -1)
    {
        Log->info("Could not read bytes");
    }
    return bytes;
}
}