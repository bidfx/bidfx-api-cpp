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

#define FAIL    -1

namespace  bidfx_public_api::tools
{

OpenSSLClient::OpenSSLClient(std::string host, int port, std::chrono::milliseconds read_timeout) : output_stream_(OutputStreamImpl(*this)), input_stream_(InputStreamImpl(*this))
{
    SSL_library_init();
    host_ = std::move(host);
    port_ = port;
    read_timeout_ = read_timeout;
    ctx_ = InitCTX();
}

void OpenSSLClient::Start()
{
    //TODO set read timeout
    server_ = OpenConnection(host_, port_);
    ssl_ = SSL_new(ctx_);      /* create new SSL connection state */
    SSL_set_fd(ssl_, server_);    /* attach the socket descriptor */
    if ( SSL_connect(ssl_) == FAIL )   /* perform the connection */
    {
        ERR_print_errors_fp(stderr);
        //TODO throw exception
        return;
    }
//    printf("Connected with %s encryption\n", SSL_get_cipher(ssl)); //TODO log
    ShowCerts(ssl_);        /* get any certs */
}

void OpenSSLClient::Close()
{
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

int OpenSSLClient::OpenConnection(std::string hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if ( (host = gethostbyname(hostname.c_str())) == NULL )
    {
        //TODO throw exception
        return -1;
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        close(sd);
        //TODO throw exception
        return -1;
    }
    return sd;
}

SSL_CTX* OpenSSLClient::InitCTX()
{
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    method = TLS_client_method(); /* Create new client-method instance */
    ctx = SSL_CTX_new(method);   /* Create new context */
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void OpenSSLClient::ShowCerts(SSL* ssl)
{   X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("Info: No client certificates configured.\n");
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

size_t OpenSSLClient::OutputStreamImpl::WriteBytes(const unsigned char* c, size_t len) {
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
    unsigned char* bytes = new unsigned char[1];
    ReadBytes(bytes, 1);

    unsigned char byte = bytes[0];
    delete[](bytes);
    return byte;
}

size_t OpenSSLClient::InputStreamImpl::ReadBytes(unsigned char *buf, size_t len)
{
    size_t bytes = SSL_read(open_ssl_client_->ssl_, buf, len);
    return bytes;
}
}