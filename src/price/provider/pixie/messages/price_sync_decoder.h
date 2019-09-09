/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef BIDFX_PUBLIC_API_PRICE_PIXIE_MESSAGES_PRICE_SYNC_DECODER_H_
#define BIDFX_PUBLIC_API_PRICE_PIXIE_MESSAGES_PRICE_SYNC_DECODER_H_

#include <memory>
#include "price_sync.h"
#include "src/tools/buffer_inflator.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::pixie::PriceSync;
using bidfx_public_api::tools::BufferInflator;

/**
 * Decodes the price sync decoder, keeping hold of the same instance of the decompressor. There must be one (and only one) instance
 * of this class per connection with the server.
 * @author Liam Asman
 */
class PriceSyncDecoder
{
private:
    BufferInflator* buffer_inflator_;
public:
    PriceSyncDecoder();
    ~PriceSyncDecoder();

    /**
     * Decodes a price sync message.
     * @param buffer the buffer to read the message from.
     * @return a new priceSync message.
     */
    PriceSync DecodePriceSync(ByteBuffer& buffer);
};
}

#endif //BIDFX_PUBLIC_API_PRICE_PIXIE_MESSAGES_PRICE_SYNC_DECODER_H_
