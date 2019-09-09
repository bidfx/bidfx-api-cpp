/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PRICE_SYNC_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PRICE_SYNC_H_

#include <memory>
#include <ostream>
#include "src/tools/byte_buffer.h"
#include "src/tools/buffer_inflator.h"
#include "syncable.h"
#include "../fields/data_dictionary.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::ByteBuffer;
using bidfx_public_api::tools::BufferInflator;

class PriceSync
{
private:
    static unsigned int kCompressionBit;

    ByteBuffer& price_updates_buffer_;
    BufferInflator& buffer_inflator_;
    uint32_t options_;
    uint32_t edition_;
    uint64_t revision_;
    uint64_t revision_time_;
    uint64_t conflation_latency_;
    uint32_t size_;
    friend std::ostream& operator<<(std::ostream& os, const bidfx_public_api::price::pixie::PriceSync& ps);
public:
    PriceSync(ByteBuffer& buffer, BufferInflator& buffer_inflator);

    void Visit(IDataDictionary& data_dictionary, Syncable& syncable);

    uint32_t GetEdition() const;

    uint64_t GetRevision() const;

    uint64_t GetRevisionTime() const;

    uint64_t GetConflationLatency() const;

    uint32_t GetSize() const;

    bool IsCompressed() const;

    std::string ToString() const;

    bool operator==(const PriceSync& that) const;
    bool operator!=(const PriceSync& that) const;
};
}
#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PRICE_SYNC_H_
