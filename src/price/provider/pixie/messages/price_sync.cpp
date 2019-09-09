/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include <sstream>
#include "src/tools/varint.h"
#include "price_sync.h"
#include "syncable.h"
#include "price_update_decoder.h"

namespace bidfx_public_api::price::pixie
{
    using bidfx_public_api::tools::Varint;

    unsigned int PriceSync::kCompressionBit = 0;

    PriceSync::PriceSync(ByteBuffer& buffer,
                         BufferInflator& buffer_inflator) : price_updates_buffer_(buffer), buffer_inflator_(buffer_inflator)
    {
        options_ = price_updates_buffer_.ReadByte();
        revision_ = Varint::ReadU64(price_updates_buffer_);
        revision_time_ = Varint::ReadU64(price_updates_buffer_);
        conflation_latency_  = Varint::ReadU64(price_updates_buffer_);
        edition_ = Varint::ReadU32(price_updates_buffer_);
        size_ = Varint::ReadU32(price_updates_buffer_);
    }

    uint32_t PriceSync::GetEdition() const
    {
        return edition_;
    }

    uint64_t PriceSync::GetRevision() const
    {
        return revision_;
    }

    uint64_t PriceSync::GetRevisionTime() const
    {
        return revision_time_;
    }

    uint64_t PriceSync::GetConflationLatency() const
    {
        return conflation_latency_;
    }

    uint32_t PriceSync::GetSize() const
    {
        return size_;
    }

    bool PriceSync::IsCompressed() const
    {
        return ((options_ & (1u << kCompressionBit)) != 0);
    }

    std::string PriceSync::ToString() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    bool PriceSync::operator==(const PriceSync& that) const
    {
        return this == &that ||
            (conflation_latency_ == that.conflation_latency_ &&
            edition_ == that.edition_ &&
            revision_ == that.revision_ &&
            revision_time_ == that.revision_time_ &&
            size_ == that.size_ &&
            price_updates_buffer_ == that.price_updates_buffer_);
    }

    bool PriceSync::operator!=(const PriceSync& that) const
    {
        return !(*this == that);
    }

    std::ostream& operator<<(std::ostream& os, const PriceSync& ps)
    {
        return os << "PriceSync("
                  << "revision=" << ps.revision_
                  << ", revisionTime=" << ps.revision_time_
                  << ", conflationLatency=" << ps.conflation_latency_
                  << ", edition=" << ps.edition_
                  << ", size=" << ps.size_
                  << ", compressed=" << (ps.IsCompressed() ? "true" : "false")
                  << ")";
    }

    void PriceSync::Visit(IDataDictionary& data_dictionary, Syncable& syncable)
    {
        if (size_ > 0)
        {
            ByteBuffer inflated_buffer = IsCompressed() ? buffer_inflator_.Inflate(price_updates_buffer_) : price_updates_buffer_;
            PriceUpdateDecoder::Visit(inflated_buffer, size_, data_dictionary, syncable);
        }
    }
}