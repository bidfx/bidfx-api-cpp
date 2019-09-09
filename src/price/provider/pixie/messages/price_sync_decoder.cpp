/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include "src/tools/zlib_buffer_inflator.h"
#include "price_sync_decoder.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::ZLIBBufferInflator;

PriceSyncDecoder::PriceSyncDecoder()
{
    buffer_inflator_ = new ZLIBBufferInflator();
}

PriceSyncDecoder::~PriceSyncDecoder()
{
    delete buffer_inflator_;
}

PriceSync PriceSyncDecoder::DecodePriceSync(ByteBuffer& buffer)
{
    return PriceSync(buffer, *buffer_inflator_);
}
}