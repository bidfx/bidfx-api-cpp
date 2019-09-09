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
#include "src/price/provider/pixie/fields/field_type.h"

namespace
{

using bidfx_public_api::price::pixie::FieldTypeEnum;
using bidfx_public_api::price::pixie::FieldTypeMethods;

TEST(FieldTypeTest, TestValueOfCodeReturnsSameFieldType)
{
    ASSERT_EQ(FieldTypeEnum::DOUBLE, FieldTypeMethods::ValueOf((int) FieldTypeEnum::DOUBLE));
    ASSERT_EQ(FieldTypeEnum::INTEGER, FieldTypeMethods::ValueOf((int) FieldTypeEnum::INTEGER));
    ASSERT_EQ(FieldTypeEnum::LONG, FieldTypeMethods::ValueOf((int) FieldTypeEnum::LONG));
    ASSERT_EQ(FieldTypeEnum::STRING, FieldTypeMethods::ValueOf((int) FieldTypeEnum::STRING));
}

TEST(FieldTypeTest, TestValueOfUnrecognisedCodeReturnsUnrecognisedFieldType)
{
    ASSERT_EQ(FieldTypeEnum::UNRECOGNISED, FieldTypeMethods::ValueOf((int) FieldTypeEnum::UNRECOGNISED));
}

TEST(FieldTypeTest, TestValueOfInvalidCodeReturnsUnrecognisedFieldType)
{
    ASSERT_EQ(FieldTypeEnum::UNRECOGNISED, FieldTypeMethods::ValueOf('x'));
    ASSERT_EQ(FieldTypeEnum::UNRECOGNISED, FieldTypeMethods::ValueOf('3'));
    ASSERT_EQ(FieldTypeEnum::UNRECOGNISED, FieldTypeMethods::ValueOf(','));
}

}