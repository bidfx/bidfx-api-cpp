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

#include "gtest/gtest.h"
#include "src/price/provider/pixie/fields/field_encoding.h"

namespace
{

using bidfx_public_api::price::pixie::FieldEncodingEnum;
using bidfx_public_api::price::pixie::FieldEncodingMethods;

TEST(FieldEncodingTest, TestValueOfCodeReturnsSameFieldEncodingType)
{
    ASSERT_EQ(FieldEncodingEnum::NOOP, FieldEncodingMethods::ValueOf((char) FieldEncodingEnum::NOOP));
    ASSERT_EQ(FieldEncodingEnum::FIXED1, FieldEncodingMethods::ValueOf((char) FieldEncodingEnum::FIXED1));
    ASSERT_EQ(FieldEncodingEnum::FIXED2, FieldEncodingMethods::ValueOf((char) FieldEncodingEnum::FIXED2));
    ASSERT_EQ(FieldEncodingEnum::FIXED3, FieldEncodingMethods::ValueOf((char) FieldEncodingEnum::FIXED3));
    ASSERT_EQ(FieldEncodingEnum::FIXED4, FieldEncodingMethods::ValueOf((char) FieldEncodingEnum::FIXED4));
    ASSERT_EQ(FieldEncodingEnum::FIXED8, FieldEncodingMethods::ValueOf((char) FieldEncodingEnum::FIXED8));
    ASSERT_EQ(FieldEncodingEnum::FIXED16, FieldEncodingMethods::ValueOf((char) FieldEncodingEnum::FIXED16));
    ASSERT_EQ(FieldEncodingEnum::BYTE_ARRAY, FieldEncodingMethods::ValueOf((char) FieldEncodingEnum::BYTE_ARRAY));
    ASSERT_EQ(FieldEncodingEnum::VARINT_STRING, FieldEncodingMethods::ValueOf((char) FieldEncodingEnum::VARINT_STRING));
    ASSERT_EQ(FieldEncodingEnum::VARINT, FieldEncodingMethods::ValueOf((char) FieldEncodingEnum::VARINT));
}

TEST(FieldEncodingTest, TestValueOfInvalidCodeReturnsUnrecognised)
{
    ASSERT_THROW(FieldEncodingMethods::ValueOf('x'), std::invalid_argument);
}

}
