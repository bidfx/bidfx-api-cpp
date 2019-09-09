/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_TOOLS_SSL_CLIENT_H_
#define PUBLIC_API_CPP_TOOLS_SSL_CLIENT_H_

#include <cstdio>
#include "output_stream.h"
#include "input_stream.h"

namespace bidfx_public_api::tools
{

class SSLClient
{
public:
    virtual ~SSLClient() {}
    virtual void Start() = 0;
    virtual void Close() = 0;
    virtual OutputStream& GetOutputStream() = 0;
    virtual InputStream& GetInputStream() = 0;
};

}

#endif //PUBLIC_API_CPP_TOOLS_SSL_CLIENT_H_
