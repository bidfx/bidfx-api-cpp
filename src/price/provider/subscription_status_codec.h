/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_SUBSCRIPTION_STATUS_CODEC_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_SUBSCRIPTION_STATUS_CODEC_H_

#include "include/subscription_status.h"

namespace bidfx_public_api::provider
{
using bidfx_public_api::SubscriptionStatus;

class SubscriptionStatusCodec
{
public:
    static SubscriptionStatus DecodeStatus(unsigned char encoding);
};
}

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_SUBSCRIPTION_STATUS_CODEC_H_
