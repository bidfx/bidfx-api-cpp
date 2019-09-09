/**  Copyright 2019 BidFX

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

            http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */

#include <list>
#include "src/price/provider/pixie/Fields/field_def.h"
#include "src/price/provider/pixie/messages/pixie_message.h"
#include "src/price/provider/pixie/messages/pixie_message_type.h"
#include "src/price/provider/pixie/messages/data_dictionary_message.h"
#include "src/price/provider/pixie/pixie_version.h"
#include "src/tools/buffer_util.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::pixie::FieldDef;
using bidfx_public_api::price::pixie::FieldTypeEnum;
using bidfx_public_api::price::pixie::FieldEncodingEnum ;
using bidfx_public_api::tools::BufferUtil;

using namespace std::string_literals;

const unsigned char DATA_DICT_MSG_TYPE = PixieMessageType::DATA_DICTIONARY;

const std::string DATA_DICT_ENCODED_MESSAGE = "D\000\011\000DV\006\004Bid \001DV\006\004Ask \002LV\000\010BidSize \003LV\000\010AskSize \004LV\000\010BidTime \005LV\000\010AskTime \006DV\002\016OrderQuantity \011DZ\004\012NetChange \012DZ\002\016PercentChange"s;

std::vector<FieldDef> data_dict_field_defs = {FieldDef(0, FieldTypeEnum::DOUBLE, "Bid", FieldEncodingEnum::VARINT, 6),
                                                               FieldDef(1, FieldTypeEnum::DOUBLE, "Ask",
                                                                                    FieldEncodingEnum::VARINT, 6),
                                                               FieldDef(2, FieldTypeEnum::LONG, "BidSize",
                                                                                    FieldEncodingEnum::VARINT, 0),
                                                               FieldDef(3, FieldTypeEnum::LONG, "AskSize",
                                                                                    FieldEncodingEnum::VARINT, 0),
                                                               FieldDef(4, FieldTypeEnum::LONG, "BidTime",
                                                                                    FieldEncodingEnum::VARINT, 0),
                                                               FieldDef(5, FieldTypeEnum::LONG, "AskTime",
                                                                                    FieldEncodingEnum::VARINT, 0),
                                                               FieldDef(6, FieldTypeEnum::DOUBLE, "OrderQuantity",
                                                                                    FieldEncodingEnum::VARINT, 2),
                                                               FieldDef(9, FieldTypeEnum::DOUBLE, "NetChange",
                                                                                    FieldEncodingEnum::ZIGZAG, 4),
                                                               FieldDef(10, FieldTypeEnum::DOUBLE, "PercentChange",
                                                                                    FieldEncodingEnum::ZIGZAG, 2)};

DataDictionaryMessage* data_dict_message =  new DataDictionaryMessage(data_dict_field_defs, false);

TEST(DataDictionaryMessageTest, TestIsUpdateMatchesValueInConstructor)
{
    EXPECT_EQ(false, (DataDictionaryMessage(data_dict_field_defs, false)).IsUpdate());
    EXPECT_EQ(true, (DataDictionaryMessage(data_dict_field_defs, true)).IsUpdate());
}

TEST(DataDictionaryMessageTest, TestDiscardFieldTypesAreNotEncodedInADictionaryMessage)
{
    FieldDef bid(0, FieldTypeEnum::DOUBLE, "Bid");
    FieldDef discarded(2, FieldTypeEnum::DISCARD, "Discarded");
    FieldDef ask(2, FieldTypeEnum::DOUBLE, "Ask");
    std::vector<FieldDef> fields = {bid, discarded, ask};
    DataDictionaryMessage message = DataDictionaryMessage(fields, false);

    EXPECT_EQ(2, message.GetFieldDefs().size());
    EXPECT_EQ(bid, message.GetFieldDefs().at(0));
    EXPECT_EQ(ask, message.GetFieldDefs().at(1));
}

TEST(DataDictionaryMessageTest, TestUnrecognisedFieldTypesAreNotEncodedInADictionaryMessage)
{
    FieldDef bid(0, FieldTypeEnum::DOUBLE, "Bid");
    FieldDef unrecognised(2, FieldTypeEnum::UNRECOGNISED, "Unrecognised");
    FieldDef ask(2, FieldTypeEnum::DOUBLE, "Ask");
    std::vector<FieldDef> fields = {bid, unrecognised, ask};
    DataDictionaryMessage message = DataDictionaryMessage(fields, false);

    EXPECT_EQ(2, message.GetFieldDefs().size());
    EXPECT_EQ(bid, message.GetFieldDefs().at(0));
    EXPECT_EQ(ask, message.GetFieldDefs().at(1));
}

TEST(DataDictionaryMessageTest, TestDecodeConstructor)
{
    ByteBuffer buffer = BufferUtil::StringAsBuffer(DATA_DICT_ENCODED_MESSAGE);
    ASSERT_EQ(DATA_DICT_MSG_TYPE, buffer.ReadByte());

}

TEST(DataDictionaryMessageTest, TestToString)
{
    ASSERT_EQ("DataDictionary(update=false, fields=[\n"
                 "  fieldDef(FID=0, name=\"Bid\", type=DOUBLE, encoding=VARINT, scale=6)\n"
                 "  fieldDef(FID=1, name=\"Ask\", type=DOUBLE, encoding=VARINT, scale=6)\n"
                 "  fieldDef(FID=2, name=\"BidSize\", type=LONG, encoding=VARINT, scale=0)\n"
                 "  fieldDef(FID=3, name=\"AskSize\", type=LONG, encoding=VARINT, scale=0)\n"
                 "  fieldDef(FID=4, name=\"BidTime\", type=LONG, encoding=VARINT, scale=0)\n"
                 "  fieldDef(FID=5, name=\"AskTime\", type=LONG, encoding=VARINT, scale=0)\n"
                 "  fieldDef(FID=6, name=\"OrderQuantity\", type=DOUBLE, encoding=VARINT, scale=2)\n"
                 "  fieldDef(FID=9, name=\"NetChange\", type=DOUBLE, encoding=ZIGZAG, scale=4)\n"
                 "  fieldDef(FID=10, name=\"PercentChange\", type=DOUBLE, encoding=ZIGZAG, scale=2)\n"
                 "])", data_dict_message->ToString());
}

}
