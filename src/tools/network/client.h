/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_TOOLS_CLIENT_H_
#define PUBLIC_API_CPP_TOOLS_CLIENT_H_

#include <cstdio>
#include <unistd.h>
#include <netdb.h>
#include <fstream>
#include "output_stream.h"
#include "input_stream.h"

namespace bidfx_public_api::tools
{

class Client
{
public:
    virtual ~Client() = default;
    virtual void Start() = 0;
    virtual void Close() = 0;
    virtual OutputStream& GetOutputStream() = 0;
    virtual InputStream& GetInputStream() = 0;

    static int OpenConnection(const std::string& hostname, int port)
    {
        int sd;
        struct hostent *host;
        struct sockaddr_in addr{};

        if ( (host = gethostbyname(hostname.c_str())) == nullptr )
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

};

}

#endif //PUBLIC_API_CPP_TOOLS_CLIENT_H_
