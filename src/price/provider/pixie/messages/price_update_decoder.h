/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef  PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PRICE_UPDATE_DECODER_H_
#define  PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PRICE_UPDATE_DECODER_H_

#include <cstdint>
#include "src/tools/byte_buffer.h"
#include "src/price/provider/pixie/fields/data_dictionary.h"
#include "syncable.h"
#include "include/price/price_field.h"

namespace bidfx_public_api::price::pixie
{

/**
 * Extract price updates from a price sync message.
 * @author Liam Asman
 */
class PriceUpdateDecoder
{
private:
    static std::map<std::string, PriceField> DecodePriceUpdate(ByteBuffer& buffer, IDataDictionary& data_dictionary);
    static FieldDef GetFieldDef(ByteBuffer& buffer, IDataDictionary& data_dictionary,
                                std::map<std::string, PriceField>& priceMap, uint32_t fid);
    static void DecodePriceStatus(ByteBuffer& buffer, uint32_t sid, Syncable& syncable);
    static PriceField DecodeAsDouble(ByteBuffer& buffer, FieldDef& field_def);
    static PriceField DecodeAsLong(ByteBuffer& buffer, FieldDef& field_def);
    static PriceField DecodeAsInteger(ByteBuffer& buffer, FieldDef& field_def);
    static PriceField DecodeAsString(ByteBuffer& buffer);
public:
    static void Visit(ByteBuffer& buffer, uint32_t size, IDataDictionary& data_dictionary,
                      Syncable& syncable);
    static PriceField DecodeField(ByteBuffer& buffer, FieldDef& field_def);

    static void SkipFieldValue(ByteBuffer& buffer, FieldDef& field_def);
};
}
#endif // PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PRICE_UPDATE_DECODER_H_
