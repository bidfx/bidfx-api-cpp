/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_ENCODING_UTIL_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_ENCODING_UTIL_H_

#include <cstdint>
#include <vector>

namespace bidfx_public_api::price::pixie
{
class EncodingUtil
{
private:
    static const std::vector<uint64_t> POWERS_OF_TEN;

public:
    static uint64_t PowerOfTen(uint32_t n);

    static double DecodeDecimal(int64_t value, int32_t scale);
};
}

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_ENCODING_UTIL_H_
