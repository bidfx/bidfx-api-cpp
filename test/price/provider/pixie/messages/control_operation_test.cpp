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

#include "src/price/provider/pixie/messages/control_operation.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::pixie::ControlOperation;
using bidfx_public_api::price::pixie::ControlOperationEnum;

TEST(ControlOperationTest, TestGetCode)
{
    ASSERT_EQ('R', ControlOperation::GetCode(ControlOperationEnum::REFRESH));
    ASSERT_EQ('T', ControlOperation::GetCode(ControlOperationEnum::TOGGLE));
}

TEST(ControlOperationTest, TestvalueOf)
{
    ASSERT_EQ(ControlOperationEnum::REFRESH, ControlOperation::FromCode('R'));
    ASSERT_EQ(ControlOperationEnum::TOGGLE, ControlOperation::FromCode('T'));
}

TEST(ControlOperationTest, TestValueOfUnkownCode)
{
    ASSERT_THROW(ControlOperation::FromCode('X'), std::invalid_argument);
}

}