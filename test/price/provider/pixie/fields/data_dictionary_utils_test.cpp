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

#include <vector>
#include "src/price/provider/pixie/fields/field_def.h"
#include "src/price/provider/pixie/fields/extendable_data_dictionary.h"
#include "src/price/provider/pixie/fields/data_dictionary_utils.h"
#include "gtest/gtest.h"

namespace
{

using bidfx_public_api::price::pixie::FieldDef;
using bidfx_public_api::price::pixie::FieldEncodingEnum;
using bidfx_public_api::price::pixie::FieldTypeEnum;
using bidfx_public_api::price::pixie::ExtendableDataDictionary;
using bidfx_public_api::price::pixie::DataDictionaryUtils;

FieldDef bid(0, FieldTypeEnum::DOUBLE, "Bid", FieldEncodingEnum::FIXED1, 1);
FieldDef ask(1, FieldTypeEnum::DOUBLE, "Ask", FieldEncodingEnum::FIXED1, 1);
FieldDef bid_size(2, FieldTypeEnum::LONG, "BidSize", FieldEncodingEnum::FIXED1, 1);
FieldDef ask_size(3, FieldTypeEnum::LONG, "AskSize", FieldEncodingEnum::FIXED1, 1);

TEST(DataDictionaryUtilsTest, TestAddAllFields)
{
    ExtendableDataDictionary data_dictionary = ExtendableDataDictionary();
    data_dictionary.AddFieldDef(bid);
    data_dictionary.AddFieldDef(ask);
    std::vector<FieldDef> inter_vector = {bid_size, ask_size};
    DataDictionaryUtils::AddAllFields(data_dictionary, inter_vector);
    std::vector<FieldDef> actual = {bid, ask, bid_size, ask_size};
    ASSERT_TRUE(actual == data_dictionary.AllFieldDefs());
}

TEST(DataDictionaryUtilsTest, TestAFieldDefIsValidIfAllComponentsAreSet)
{
    FieldDef field_def(1, FieldTypeEnum::INTEGER, "Bid", FieldEncodingEnum::FIXED4, 4);
    ASSERT_TRUE(DataDictionaryUtils::IsValid(field_def));
}

TEST(DataDictionaryUtilsTest, TestAFieldDefIsNotValidIfFidIsNotSet)
{
    FieldDef field_def(0, FieldTypeEnum::INTEGER, "Bid", FieldEncodingEnum::NOT_DEFINED, 0);
    ASSERT_FALSE(DataDictionaryUtils::IsValid(field_def));
}

TEST(DataDictionaryUtilsTest, TestAFieldDefIsNotValidWithNegativeFid)
{
    ASSERT_FALSE(DataDictionaryUtils::IsValid(FieldDef(-1, FieldTypeEnum::UNRECOGNISED, "Invalid")));
}

TEST(DataDictionaryUtilsTest, TestAFieldDefIsNotValidIfNameIsNotSet)
{
    FieldDef field_def(1, FieldTypeEnum::INTEGER, "", FieldEncodingEnum::NOT_DEFINED, 0);
    ASSERT_FALSE(DataDictionaryUtils::IsValid(field_def));
}

TEST(DataDictionaryUtilsTest, TestAFieldDefIsNotValidIfTypeIsNotSet)
{
    FieldDef field_def(1, FieldTypeEnum::UNRECOGNISED, "Bid", FieldEncodingEnum::NOT_DEFINED, 0);
    ASSERT_FALSE(DataDictionaryUtils::IsValid(field_def));
}

}
