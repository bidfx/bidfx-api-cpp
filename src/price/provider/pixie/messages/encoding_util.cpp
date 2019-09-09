/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include <stdexcept>
#include "encoding_util.h"

namespace bidfx_public_api::price::pixie
{
const std::vector<uint64_t> EncodingUtil::POWERS_OF_TEN = {
1L, 10L, 100L, 1000L, 10000L, 100000L, 1000000L, 10000000L, 100000000L, 1000000000L,
10000000000L, 100000000000L, 1000000000000L, 10000000000000L, 100000000000000L,
1000000000000000L, 10000000000000000L, 100000000000000000L, 1000000000000000000L};

double EncodingUtil::DecodeDecimal(int64_t value, int32_t scale)
{
    return scale == 0 ? value : value / (double)PowerOfTen(scale);
}

uint64_t EncodingUtil::PowerOfTen(uint32_t n)
{
    if(n >= POWERS_OF_TEN.size())
    {
        throw std::invalid_argument("no power of 10 for scale: " + n);
    }
    return POWERS_OF_TEN[n];
}
}