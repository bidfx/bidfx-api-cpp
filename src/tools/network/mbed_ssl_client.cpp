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

#if defined(__CYGWIN__) || defined(_WIN32)
#include <cstdio>
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib, "crypt32.lib")
#else
#endif

#include <sstream>
#include <cstring>
#include <iostream>
#include <chrono>
#include "mbed_ssl_client.h"

#define DEBUG_LEVEL 1

namespace bidfx_public_api::tools
{

MBedSSLClient::MBedSSLClient(std::string host, int port, std::chrono::milliseconds read_timeout)
        : output_stream_(OutputStreamImpl(*this)), input_stream_(InputStreamImpl(*this))
{
    host_ = host;
    port_ = port;
    read_timeout_ = read_timeout;
}

void MBedSSLClient::Start()
{
    InitializeSessionData();
    InitializeCertificates();
    StartConnection();
    SetupConfigDefaults();
    Handshake();
    VerifyCertificates();
}

void MBedSSLClient::InitializeSessionData()
{
    int ret;

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold( DEBUG_LEVEL );
#endif

    /*
    * 0. Initialize the RNG and the session data
    */
    mbedtls_net_init(&server_fd_);
    mbedtls_ssl_init(&ssl_);
    mbedtls_ssl_config_init(&conf_);
    mbedtls_ssl_conf_read_timeout(&conf_, read_timeout_.count());
    mbedtls_x509_crt_init(&cacert_);
    mbedtls_ctr_drbg_init(&ctr_drbg_);

    mbedtls_entropy_init(&entropy_);
    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg_, mbedtls_entropy_func, &entropy_,
                                     (const unsigned char *) pers,
                                     strlen(pers))) != 0)
    {
        std::stringstream error_message;
        error_message << "mbedTLS initialization failure: mbedtls_ctr_drbg_seed returned " << ret << ": " << GetErrorMessage(ret);
        throw std::ios_base::failure(error_message.str());
    }
}

void MBedSSLClient::InitializeCertificates()
{
#if defined(__CYGWIN__) || defined(_WIN32)
    HANDLE hCertStore;
    PCCERT_CONTEXT p_cert_context = nullptr;

    if (!(hCertStore = CertOpenSystemStore(0, "ROOT")))
    {
        std::stringstream error_message;
        error_message << "mbedTLS - process failed to open certificate store ";
        throw std::ios_base::failure(error_message.str());
    }

    while((p_cert_context = CertEnumCertificatesInStore(hCertStore, p_cert_context)))
    {
        if (GetLastError() == CRYPT_E_NOT_FOUND)
        {
            break;
        }

        if (p_cert_context->dwCertEncodingType != X509_ASN_ENCODING)
        {
            continue;
        }

        int ret = mbedtls_x509_crt_parse(&cacert_, p_cert_context->pbCertEncoded, p_cert_context->cbCertEncoded);

        if (ret < 0)
        {
            std::stringstream error_message;
            error_message << "mbedTLS initialization failure: mbedtls_x509_crt_parse returned -0x" << std::hex << ret << ": " << GetErrorMessage(ret);
            throw std::ios_base::failure(error_message.str());
        }
    }
#else

#endif
}

void MBedSSLClient::StartConnection()
{
    int ret;

    const char* port_string = std::to_string(port_).c_str();

    if ((ret = mbedtls_net_connect(&server_fd_, host_.c_str(),
                                   port_string, MBEDTLS_NET_PROTO_TCP)) != 0)
    {
        std::stringstream error_message;
        error_message << "mbedTLS connection failure: mbedtls_net_connect returned " << ret << ": " << GetErrorMessage(ret);
        throw std::ios_base::failure(error_message.str());
    }
}

void MBedSSLClient::SetupConfigDefaults()
{
    int ret;

    if ((ret = mbedtls_ssl_config_defaults(&conf_,
                                           MBEDTLS_SSL_IS_CLIENT,
                                           MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        std::stringstream error_message;
        error_message << "mbedTLS setup failure: mbedtls_ssl_config_defaults returned " << ret << ": " << GetErrorMessage(ret);
        throw std::ios_base::failure(error_message.str());
    }

    mbedtls_ssl_conf_authmode(&conf_, MBEDTLS_SSL_VERIFY_OPTIONAL);
    mbedtls_ssl_conf_ca_chain(&conf_, &cacert_, nullptr);
    mbedtls_ssl_conf_rng(&conf_, mbedtls_ctr_drbg_random, &ctr_drbg_);

    if ((ret = mbedtls_ssl_setup(&ssl_, &conf_)) != 0)
    {
        std::stringstream error_message;
        error_message << "mbedTLS setup failure: mbedtls_ssl_setup returned " << ret << ": " << GetErrorMessage(ret);
        throw std::ios_base::failure(error_message.str());
    }

    if ((ret = mbedtls_ssl_set_hostname(&ssl_, host_.c_str())) != 0)
    {
        std::stringstream error_message;
        error_message << "mbedTLS setup failure: mbedtls_ssl_set_hostname returned " << ret << ": " << GetErrorMessage(ret);
        throw std::ios_base::failure(error_message.str());
    }

    mbedtls_ssl_set_bio(&ssl_, &server_fd_, mbedtls_net_send, mbedtls_net_recv, nullptr);
}

void MBedSSLClient::Handshake()
{
    int ret;

    while ((ret = mbedtls_ssl_handshake(&ssl_)) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            std::stringstream error_message;
            error_message << "mbedTLS setup failure: mbedtls_ssl_handshake returned -0x" << std::hex << ret << ": " << GetErrorMessage(ret);
            throw std::ios_base::failure(error_message.str());
        }
    }
}

void MBedSSLClient::VerifyCertificates()
{
    /* In real life, we probably want to bail out when ret != 0 */
    if ((flags_ = mbedtls_ssl_get_verify_result(&ssl_)) != 0)
    {
        char vrfy_buf[512];
        mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", flags_);
    }
}

void MBedSSLClient::Close()
{
    mbedtls_ssl_close_notify(&ssl_);

    mbedtls_net_free(&server_fd_);

    mbedtls_x509_crt_free(&cacert_);
    mbedtls_ssl_free(&ssl_);
    mbedtls_ssl_config_free(&conf_);
    mbedtls_ctr_drbg_free(&ctr_drbg_);
    mbedtls_entropy_free(&entropy_);
}

char* MBedSSLClient::GetErrorMessage(int value)
{
    char *error_buf = new char[100];
    mbedtls_strerror(value, error_buf, 100);
    return error_buf;
}

OutputStream& MBedSSLClient::GetOutputStream()
{
    return output_stream_;
}

InputStream& MBedSSLClient::GetInputStream()
{
    return input_stream_;
}

/************************
 * Output Stream
 ***********************/
MBedSSLClient::OutputStreamImpl::OutputStreamImpl(MBedSSLClient& m_bed_client)
{
    m_bed_client_ = &m_bed_client;
}

size_t MBedSSLClient::OutputStreamImpl::WriteByte(const unsigned char c)
{
    return WriteBytes(&c, 1);
}

size_t MBedSSLClient::OutputStreamImpl::WriteBytes(const unsigned char* c, size_t len)
{
    size_t len_orig = len;
    int ret;

    do
    {
        ret = mbedtls_ssl_write(&m_bed_client_->ssl_, c, len);
        if (ret < 0 && ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            std::stringstream error_message;
            error_message << "connection write failure: mbedtls_ssl_write returned " << ret << ": " << m_bed_client_->GetErrorMessage(ret);
            throw std::ios_base::failure(error_message.str());
        }
        else
        {
            len -= ret;
            c += ret;
        }
    } while (ret != len);

    return len_orig;
}

size_t MBedSSLClient::OutputStreamImpl::WriteString(const std::string& str)
{
    size_t len = str.size();
    return WriteBytes(reinterpret_cast<const unsigned char*>(str.data()), len);
}

/************************
 * Input Stream
 ***********************/
MBedSSLClient::InputStreamImpl::InputStreamImpl(MBedSSLClient& m_bed_client)
{
    m_bed_client_ = &m_bed_client;
}

unsigned char MBedSSLClient::InputStreamImpl::ReadByte()
{
    unsigned char* bytes = new unsigned char[1];
    ReadBytes(bytes, 1);

    unsigned char byte = bytes[0];
    delete(bytes);
    return byte;
}

size_t MBedSSLClient::InputStreamImpl::ReadBytes(unsigned char* output_buf, size_t len)
{
    size_t ret = static_cast<size_t>(mbedtls_ssl_read(&m_bed_client_->ssl_, output_buf, len));

    if(ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
        return 0;

    if(ret == MBEDTLS_ERR_NET_CONN_RESET || ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY  || ret < 0)
    {
        std::stringstream error_ss;
        error_ss << "[ERROR] connection read failure: mbedtls_ssl_read returned " << ret << ": " << m_bed_client_->GetErrorMessage(ret);
        throw std::ios_base::failure(error_ss.str());
    }

    return ret;
}

}