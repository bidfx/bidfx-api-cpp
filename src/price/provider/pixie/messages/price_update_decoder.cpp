/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include <sstream>
#include <memory>
#include <iostream>
#include "src/price/provider/pixie/fields/data_dictionary_utils.h"
#include "include/exceptions/bid_fx_exception.h"
#include "include/exceptions/decoding_exception.h"
#include "src/tools/varint.h"
#include "encoding_util.h"
#include "price_update_decoder.h"
#include "src/price/provider/subscription_status_codec.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::Varint;
using bidfx_public_api::provider::SubscriptionStatusCodec;
using bidfx_public_api::exceptions::DecodingException;

void PriceUpdateDecoder::Visit(ByteBuffer& buffer, uint32_t size, IDataDictionary& data_dictionary,
                               Syncable& syncable)
{
    for (uint32_t i = 0; i < size; i++)
    {
        const char type = buffer.ReadByte();
        uint32_t sid = Varint::ReadU32(buffer);
        switch (type)
        {
            case 'f': //FULL (image)
                syncable.PriceImage(sid, DecodePriceUpdate(buffer, data_dictionary));
                break;
            case 'p': // PARTIAL (update)
                syncable.PriceUpdate(sid, DecodePriceUpdate(buffer, data_dictionary));
                break;
            case 's': // STATUS
                DecodePriceStatus(buffer, sid, syncable);
                break;
            case 'G': // FULL GRID IMAGE
                // Grids not supported in cpp api
                break;
            case 'g': // GRID UPDATE
                // Grids not supported in cpp api
                break;
            default:
                std::stringstream err_msg;
                err_msg << "unexpected price update type '" << type << "'" << std::endl;
                std::cout << "[ERROR] " << err_msg.str();
                throw std::invalid_argument(err_msg.str());
        }
    }
}

PriceField PriceUpdateDecoder::DecodeField(ByteBuffer& buffer, FieldDef& field_def)
{
    switch (field_def.type_)
    {
        case DOUBLE:
            return DecodeAsDouble(buffer, field_def);
        case INTEGER:
            return DecodeAsInteger(buffer, field_def);
        case LONG:
            return DecodeAsLong(buffer, field_def);
        case STRING:
            return DecodeAsString(buffer);
        default:
            SkipFieldValue(buffer, field_def);
            return PriceField();
    }
}

PriceField PriceUpdateDecoder::DecodeAsDouble(ByteBuffer& buffer, FieldDef& field_def)
{
    switch(field_def.encoding_)
    {
        case FIXED4:
            return PriceField((double) buffer.ReadFloat());
        case FIXED8:
            return PriceField(buffer.ReadDouble());
        case VARINT:
            return PriceField(EncodingUtil::DecodeDecimal(Varint::ReadU64(buffer), field_def.scale_));
        case ZIGZAG:
            return PriceField(EncodingUtil::DecodeDecimal(Varint::ZigzagToLong(Varint::ReadU64(buffer)), field_def.scale_));
    }
    SkipFieldValue(buffer, field_def);
    return PriceField();
}

PriceField PriceUpdateDecoder::DecodeAsLong(ByteBuffer& buffer, FieldDef& field_def)
{
    switch(field_def.encoding_)
    {
        case FIXED1:
            return PriceField((int64_t) buffer.ReadByte());
        case FIXED2:
            return PriceField((int64_t) buffer.ReadShort());
        case FIXED3:
            return PriceField((int64_t) buffer.ReadMedium());
        case FIXED4:
            return PriceField((int64_t) buffer.ReadInt());
        case FIXED8:
            return PriceField(buffer.ReadLong());
        case VARINT:
            return PriceField((int64_t) (Varint::ReadU64(buffer) * EncodingUtil::PowerOfTen(field_def.scale_)));
        case ZIGZAG:
            return PriceField(static_cast<int64_t>(Varint::ZigzagToLong(tools::Varint::ReadU64(buffer))));
    }
    SkipFieldValue(buffer, field_def);
    return PriceField();

}

PriceField PriceUpdateDecoder::DecodeAsInteger(ByteBuffer& buffer, FieldDef& field_def)
{
    switch (field_def.encoding_)
    {
        case FIXED1:
            return PriceField((int32_t) buffer.ReadByte());
        case FIXED2:
            return PriceField((int32_t) buffer.ReadShort());
        case FIXED3:
            return PriceField(buffer.ReadMedium());
        case FIXED4:
            return PriceField(buffer.ReadInt());
        case FIXED8:
            return PriceField((int32_t) buffer.ReadLong());
        case VARINT:
            return PriceField((int32_t) tools::Varint::ReadU32(buffer) * (int32_t) EncodingUtil::PowerOfTen(field_def.scale_));
        case ZIGZAG:
            return PriceField(Varint::ZigzagToInt(tools::Varint::ReadU32(buffer)));
    }
    SkipFieldValue(buffer, field_def);
    return PriceField();
}

PriceField PriceUpdateDecoder::DecodeAsString(ByteBuffer& buffer)
{
    return PriceField(Varint::ReadString(buffer));
}

void PriceUpdateDecoder::SkipFieldValue(ByteBuffer& buffer, FieldDef& field_def)
{
    uint32_t bytes_to_skip = Varint::ReadU32(buffer);
    buffer.SkipBytes(bytes_to_skip);
}

void PriceUpdateDecoder::DecodePriceStatus(ByteBuffer& buffer, uint32_t sid, Syncable& syncable)
{
    SubscriptionStatus status  = SubscriptionStatusCodec::DecodeStatus(buffer.ReadByte());
    std::string explanation = tools::Varint::ReadString(buffer);
    syncable.PriceStatus(sid, status, explanation);
}

std::map<std::string, PriceField> PriceUpdateDecoder::DecodePriceUpdate(ByteBuffer& buffer, IDataDictionary& data_dictionary)
{
    uint32_t size = Varint::ReadU32(buffer);
    std::map<std::string, PriceField> price_map;
    for (uint32_t i = 0; i < size; i++)
    {
        uint32_t fid = Varint::ReadU32(buffer);
        if (fid == FieldDef::REFERENCING_FID)
        {
            uint32_t referenced_fid = Varint::ReadU32(buffer);
            FieldDef field_def = GetFieldDef(buffer, data_dictionary, price_map, referenced_fid);
            price_map.emplace(field_def.name_, PriceField::null);
        }
        else
        {
            FieldDef field_def = GetFieldDef(buffer, data_dictionary, price_map, fid);
            price_map.emplace(field_def.name_, DecodeField(buffer, field_def));
        }
    }
    return price_map;
}

FieldDef PriceUpdateDecoder::GetFieldDef(ByteBuffer& buffer, IDataDictionary& data_dictionary,
                                         std::map<std::string, PriceField>& price_map, uint32_t fid)
{
    std::optional<FieldDef> fieldDef = data_dictionary.FieldDefByFid(fid);
    if (!fieldDef)
    {
        throw DecodingException("attempted to get a field def which does not exist");
    }
    return *fieldDef;
}

}