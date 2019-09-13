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

#include <stdexcept>
#include "src/price/provider/pixie/fields/field_def.h"
#include "src/price/provider/pixie/fields/field_encoding.h"
#include "src/price/provider/pixie/fields/extendable_data_dictionary.h"
#include "src/price/provider/pixie/fields/data_dictionary_utils.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::pixie::FieldDef;
using bidfx_public_api::price::pixie::FieldTypeEnum;
using bidfx_public_api::price::pixie::FieldEncodingEnum;
using bidfx_public_api::price::pixie::ExtendableDataDictionary;


FieldDef bid2(0, FieldTypeEnum::DOUBLE, "Bid", FieldEncodingEnum::FIXED1, 1);
FieldDef ask2(1, FieldTypeEnum::DOUBLE, "Ask", FieldEncodingEnum::FIXED1, 1);
FieldDef bid_size2(2, FieldTypeEnum::LONG, "BidSize", FieldEncodingEnum::FIXED1, 1);
FieldDef ask_size2(3, FieldTypeEnum::LONG, "AskSize", FieldEncodingEnum::FIXED1, 1);
FieldDef status2(9, FieldTypeEnum::LONG, "Status", FieldEncodingEnum::FIXED1, 1);

ExtendableDataDictionary CreateExtendableDataDictionary();

ExtendableDataDictionary extendable_data_dictionary = CreateExtendableDataDictionary();

ExtendableDataDictionary CreateExtendableDataDictionary()
{
    ExtendableDataDictionary result = ExtendableDataDictionary();
    result.AddFieldDef(bid2);
    result.AddFieldDef(ask2);
    result.AddFieldDef(bid_size2);
    result.AddFieldDef(ask_size2);
    result.AddFieldDef(status2);
    return result;
}

TEST(ExtendedDataDictionaryTest, TestNewDictionaryIsInitiallyEmpty)
{
    ASSERT_EQ(0, ExtendableDataDictionary().size());
}

TEST(ExtendedDataDictionaryTest, TestLookupByFid)
{
    EXPECT_EQ(bid2, extendable_data_dictionary.FieldDefByFid(bid2.fid_));
    EXPECT_EQ(ask2, extendable_data_dictionary.FieldDefByFid(ask2.fid_));
    EXPECT_EQ(bid_size2, extendable_data_dictionary.FieldDefByFid(bid_size2.fid_));
    EXPECT_EQ(ask_size2, extendable_data_dictionary.FieldDefByFid(ask_size2.fid_));
    EXPECT_EQ(status2, extendable_data_dictionary.FieldDefByFid(status2.fid_));
}

TEST(ExtendedDataDictionaryTest, TestLookupByFidReturnsNoValueFieldDefForUnknownFids)
{
    EXPECT_FALSE(extendable_data_dictionary.FieldDefByFid(6));
    EXPECT_FALSE(extendable_data_dictionary.FieldDefByFid(15));
    EXPECT_FALSE(extendable_data_dictionary.FieldDefByFid(100));
}

TEST(ExtendedDataDictionaryTest, TestLookupByName)
{
    EXPECT_EQ(bid2, extendable_data_dictionary.FieldDefByName(bid2.name_));
    EXPECT_EQ(ask2, extendable_data_dictionary.FieldDefByName(ask2.name_));
    EXPECT_EQ(bid_size2, extendable_data_dictionary.FieldDefByName(bid_size2.name_));
    EXPECT_EQ(ask_size2, extendable_data_dictionary.FieldDefByName(ask_size2.name_));
    EXPECT_EQ(status2, extendable_data_dictionary.FieldDefByName(status2.name_));
}

TEST(ExtendedDataDictionaryTest, TestLookupByNameReturnsNoValueForUnknownFieldNames)
{
    EXPECT_FALSE(extendable_data_dictionary.FieldDefByName(""));
    EXPECT_FALSE(extendable_data_dictionary.FieldDefByName("Unknown"));
    EXPECT_FALSE(extendable_data_dictionary.FieldDefByName("SillyFieldName"));
}

TEST(ExtendedDataDictionaryTest, TestAllFieldsAreReturnInFidSequenceOrder)
{
    std::vector<FieldDef> actual = {bid2, ask2, bid_size2, ask_size2, status2};
    ASSERT_EQ(actual, extendable_data_dictionary.AllFieldDefs());
}

TEST(ExtendedDataDictionaryTest,
     TestAllFieldsAreReturnInFidSequenceOrderEvenWhenAddedInADifferentOrder)
{
    ExtendableDataDictionary data_dictionary = ExtendableDataDictionary();
    data_dictionary.AddFieldDef(ask2);
    data_dictionary.AddFieldDef(bid_size2);
    data_dictionary.AddFieldDef(status2);
    data_dictionary.AddFieldDef(bid2);
    data_dictionary.AddFieldDef(ask_size2);
    std::vector<FieldDef> actual = {bid2, ask2, bid_size2, ask_size2, status2};
    ASSERT_EQ(actual, data_dictionary.AllFieldDefs());
}

TEST(ExtendedDataDictionaryTest, TestNewFieldsDefsCanBeAddedToTheEndOfTheDictionary)
{
    ExtendableDataDictionary extendable_data_dictionary1 = CreateExtendableDataDictionary();
    FieldDef endSize(5000, FieldTypeEnum::LONG, "EndSize", FieldEncodingEnum::FIXED1, 1);
    ASSERT_FALSE(extendable_data_dictionary1.FieldDefByFid(endSize.fid_));
    ASSERT_FALSE(extendable_data_dictionary1.FieldDefByName(endSize.name_));
    extendable_data_dictionary1.AddFieldDef(endSize);
    ASSERT_EQ(endSize, extendable_data_dictionary1.FieldDefByFid(endSize.fid_));
    ASSERT_EQ(endSize, extendable_data_dictionary1.FieldDefByName(endSize.name_));
}

TEST(ExtendedDataDictionaryTest, TestNewFieldsDefsCanBeAddedToTheMiddleOfTheDictionary)
{
    ExtendableDataDictionary extendable_data_dictionary2 = CreateExtendableDataDictionary();
    FieldDef mid(6, FieldTypeEnum::DOUBLE, "Mid", FieldEncodingEnum::FIXED1, 1);
    ASSERT_FALSE(extendable_data_dictionary2.FieldDefByFid(mid.fid_));
    ASSERT_FALSE(extendable_data_dictionary2.FieldDefByName(mid.name_));
    extendable_data_dictionary2.AddFieldDef(mid);
    ASSERT_EQ(mid, extendable_data_dictionary2.FieldDefByFid(mid.fid_));
    ASSERT_EQ(mid, extendable_data_dictionary2.FieldDefByName(mid.name_));
}

TEST(ExtendedDataDictionaryTest, TestFieldDefinitionsCanBeHaveTheirNameRedefined)
{
    ExtendableDataDictionary extendable_data_dictionary3 = CreateExtendableDataDictionary();
    FieldDef redefined(bid2.fid_, FieldTypeEnum::DOUBLE, "BidPrice", FieldEncodingEnum::FIXED1, 1);
    extendable_data_dictionary3.AddFieldDef(redefined);
    ASSERT_EQ(redefined, extendable_data_dictionary3.FieldDefByFid(redefined.fid_));
    ASSERT_EQ(redefined, extendable_data_dictionary3.FieldDefByName(redefined.name_));
}

TEST(ExtendedDataDictionaryTest, WhenAFieldNameIsRedefinedItsOldDefinitionIsRemoved)
{
    ExtendableDataDictionary extendable_data_dictionary4 = CreateExtendableDataDictionary();
    FieldDef redefined(bid2.fid_, FieldTypeEnum::DOUBLE, "BidPrice", FieldEncodingEnum::FIXED1, 1);
    extendable_data_dictionary4.AddFieldDef(redefined);
    ASSERT_FALSE(extendable_data_dictionary4.FieldDefByName(bid2.name_));
    std::vector<FieldDef> actual = {redefined, ask2, bid_size2, ask_size2, status2};
    ASSERT_EQ(actual, extendable_data_dictionary4.AllFieldDefs());
}

TEST(ExtendedDataDictionaryTest, TestFieldDefinitionsCanBeHaveTheirFidRedefined)
{
    ExtendableDataDictionary extendable_data_dictionary5 = CreateExtendableDataDictionary();
    FieldDef redefined(4, bid2.type_, bid2.name_, FieldEncodingEnum::FIXED1, 1);
    extendable_data_dictionary5.AddFieldDef(redefined);
    ASSERT_EQ(redefined, extendable_data_dictionary5.FieldDefByFid(redefined.fid_));
    ASSERT_EQ(redefined, extendable_data_dictionary5.FieldDefByName(redefined.name_));
}

TEST(ExtendedDataDictionaryTest, WhenAFieldFidIsRedefinedItsOldDefinitionIsRemoved)
{
    ExtendableDataDictionary extendable_data_dictionary6 = CreateExtendableDataDictionary();
    FieldDef redefined(4, bid2.type_, bid2.name_, FieldEncodingEnum::FIXED1, 1);
    extendable_data_dictionary6.AddFieldDef(redefined);
    ASSERT_FALSE(extendable_data_dictionary6.FieldDefByFid(bid2.fid_));
    std::vector<FieldDef> actual = {ask2, bid_size2, ask_size2, redefined, status2};
    ASSERT_EQ(actual, extendable_data_dictionary6.AllFieldDefs());
}

TEST(ExtendedDataDictionaryTest, TestNextFreeFidForAnEmptyDataDictionaryIsZero)
{
    ASSERT_EQ(0, ExtendableDataDictionary().NextFreeFid());
}

TEST(ExtendedDataDictionaryTest,
     TestNextFreeFidForAOneEntryDataDictionaryIsOneMoreThanTheLargestExistingFid)
{
    ExtendableDataDictionary dataDictionary = ExtendableDataDictionary();
    dataDictionary.AddFieldDef(ask2);
    ASSERT_EQ(ask2.fid_ + 1, dataDictionary.NextFreeFid());
}

TEST(ExtendedDataDictionaryTest,
     TestNextFreeFidForATwoEntryDataDictionaryIsOneMoreThanTheLargestExistingFid)
{
    ExtendableDataDictionary dataDictionary = ExtendableDataDictionary();
    dataDictionary.AddFieldDef(ask2);
    dataDictionary.AddFieldDef(ask_size2);
    ASSERT_EQ(ask_size2.fid_ + 1, dataDictionary.NextFreeFid());
}

TEST(ExtendedDataDictionaryTest,
     TestNextFreeFidForAPopulatedDataDictionaryIsOneMoreThanTheLargestExistingFid)
{
    ExtendableDataDictionary extendable_data_dictionary8 = CreateExtendableDataDictionary();
    ASSERT_EQ(status2.fid_ + 1, extendable_data_dictionary.NextFreeFid());
    FieldDef settlement(19, FieldTypeEnum::STRING, "SettlementType", FieldEncodingEnum::FIXED1, 1);
    extendable_data_dictionary8.AddFieldDef(settlement);
    ASSERT_EQ(20, extendable_data_dictionary8.NextFreeFid());
}

TEST(ExtendedDataDictionaryTest, TestTheAbilityOfTheCollectionToGrowItsCapacity)
{
    ExtendableDataDictionary data_dictionary = ExtendableDataDictionary();
    ASSERT_EQ(0, data_dictionary.size());
    ASSERT_EQ(0, data_dictionary.NextFreeFid());
    for (int i = 0; i < 1000; i++)
    {
        FieldDef field_def(i, FieldTypeEnum::STRING,
                         "Bidder" + std::to_string(i),
                         FieldEncodingEnum::FIXED1, 1);
        data_dictionary.AddFieldDef(field_def);
        ASSERT_EQ(i + 1, data_dictionary.size());
        ASSERT_EQ(i + 1, data_dictionary.NextFreeFid());
    }
}
}
