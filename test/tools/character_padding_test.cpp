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

#include "src/tools/character_padding.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::tools::character_padding_test
{

TEST(CharacterPaddingTest, test_to_string)
{
    EXPECT_EQ("", CharacterPadding('x', 0).ToString());
    EXPECT_EQ("x", CharacterPadding('x', 1).ToString());
    EXPECT_EQ("xx", CharacterPadding('x', 2).ToString());
    EXPECT_EQ("xxxxxxxxxx", CharacterPadding('x', 10).ToString());
    EXPECT_EQ("yyyyyyyyyyyyyyyyyyyy", CharacterPadding('y', 20).ToString());
}

}