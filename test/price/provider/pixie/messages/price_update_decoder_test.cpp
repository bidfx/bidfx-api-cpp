/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include <src/tools/varint.h>
#include "src/tools/varint.h"
#include "src/tools/buffer_util.h"
#include "src/price/provider/pixie/fields/data_dictionary.h"
#include "src/price/provider/pixie/messages/price_update_decoder.h"
#include "src/price/provider/pixie/fields/field_def.h"
#include "src/tools/byte_buffer.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::ByteBuffer;
using bidfx_public_api::tools::Varint;

class PriceUpdateDecoderTest : public ::testing::Test
{
public:
    ByteBuffer buffer;
};

TEST_F(PriceUpdateDecoderTest, test_decode_double_field)
{
    double value = 34892343.2132;
    buffer.WriteDouble(value);
    FieldDef field_def = FieldDef(1, FieldTypeEnum::DOUBLE, "field");
    PriceField field = PriceUpdateDecoder::DecodeField(buffer, field_def);
    EXPECT_EQ(PriceField::DOUBLE, field.GetType());
    EXPECT_EQ(value, field.GetDouble());
}

TEST_F(PriceUpdateDecoderTest, test_decode_long_field)
{
    int64_t value = 483948038L;
    buffer.WriteLong(value);
    FieldDef field_def = FieldDef(1, FieldTypeEnum::LONG, "field");
    PriceField field = PriceUpdateDecoder::DecodeField(buffer, field_def);
    EXPECT_EQ(PriceField::LONG, field.GetType());
    EXPECT_EQ(value, field.GetLong());
}

TEST_F(PriceUpdateDecoderTest, test_decode_int_field)
{
    int32_t value = 483948038;
    buffer.WriteInt(value);
    FieldDef field_def = FieldDef(1, FieldTypeEnum::INTEGER, "field");
    PriceField field = PriceUpdateDecoder::DecodeField(buffer, field_def);
    EXPECT_EQ(PriceField::INTEGER, field.GetType());
    EXPECT_EQ(value, field.GetInt());
}

TEST_F(PriceUpdateDecoderTest, test_decode_string_field)
{
    std::string value = "this is certainly a string";
    Varint::WriteString(buffer, value);
    FieldDef field_def = FieldDef(1, FieldTypeEnum::STRING, "field");
    PriceField field = PriceUpdateDecoder::DecodeField(buffer, field_def);
    EXPECT_EQ(PriceField::STRING, field.GetType());
    EXPECT_EQ(value, field.GetString());
}

TEST_F(PriceUpdateDecoderTest, test_skip_bytes_in_case_type_is_unknown)
{
    std::string string = "ciao";
    unsigned char* unknown = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    double price = 9876.543;

    // STRING
    Varint::WriteString(buffer, string);
    // UNDECODABLE
    Varint::WriteU32(buffer, 9);
    buffer.WriteBytes(unknown, 9);
    // PRICE
    Varint::WriteU64(buffer, (uint64_t)(price * 1000000));

    FieldDef field_def_string = FieldDef(1, FieldTypeEnum::STRING, "field");
    FieldDef field_def_unknown = FieldDef(1, FieldTypeEnum::UNRECOGNISED, "field", FieldEncodingEnum::BYTE_ARRAY, 0);
    FieldDef field_def_price = FieldDef(1, FieldTypeEnum::DOUBLE, "field", FieldEncodingEnum::VARINT, 6);
    PriceField price_field_string = PriceUpdateDecoder::DecodeField(buffer, field_def_string);
    PriceField price_field_unknown = PriceUpdateDecoder::DecodeField(buffer, field_def_unknown);
    PriceField price_field_price = PriceUpdateDecoder::DecodeField(buffer, field_def_price);

    EXPECT_EQ(PriceField::STRING, price_field_string.GetType());
    EXPECT_EQ(string, price_field_string.GetString());
    EXPECT_EQ(PriceField::NOTHING, price_field_unknown.GetType());
    EXPECT_EQ(PriceField::DOUBLE, price_field_price.GetType());
    EXPECT_EQ(price, price_field_price.GetDouble());
}

TEST_F(PriceUpdateDecoderTest, test_decode_price_field)
{
    double price = 43.45664;
    uint64_t priceTimesMillion = 43456640L;
    Varint::WriteU64(buffer, priceTimesMillion);
    FieldDef field_def = FieldDef(1, FieldTypeEnum::DOUBLE, "field", FieldEncodingEnum::VARINT, 6);
    PriceField price_field = PriceUpdateDecoder::DecodeField(buffer, field_def);
    EXPECT_EQ(PriceField::DOUBLE, price_field.GetType());
    EXPECT_EQ(price, price_field.GetDouble());
}

TEST_F(PriceUpdateDecoderTest, test_decode_size_quantity)
{
    uint64_t size = 3823908209333543L;
    Varint::WriteU64(buffer, size);
    FieldDef field_def = FieldDef(1, FieldTypeEnum::LONG, "field", FieldEncodingEnum::VARINT, 0);
    PriceField price_field = PriceUpdateDecoder::DecodeField(buffer, field_def);
    EXPECT_EQ(PriceField::LONG, price_field.GetType());
    EXPECT_EQ(size, price_field.GetLong());
}

}