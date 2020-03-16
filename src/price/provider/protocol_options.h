/**
 * Copyright 2020 BidFX Systems Ltd. All rights reserved.
 */

#ifndef BIDFX_PUBLIC_API_CPP_PROTOCOL_OPTIONS_H
#define BIDFX_PUBLIC_API_CPP_PROTOCOL_OPTIONS_H

#include <chrono>

namespace bidfx_public_api::price
{

class ProtocolOptions
{
public:
    virtual ~ProtocolOptions() = default;

    virtual std::string GetName() const = 0;
    virtual bool CompressSubscriptions() const = 0;
    virtual std::chrono::milliseconds GetHeartbeatInterval() const = 0;
    virtual int GetVersion() const = 0;
    virtual std::chrono::milliseconds GetIdleInterval() const = 0;
    virtual int GetMinti() const = 0;
    virtual std::string GetProtocolSignature() const = 0;
};

}

#endif //BIDFX_PUBLIC_API_CPP_PROTOCOL_OPTIONS_H
