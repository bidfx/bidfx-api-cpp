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

#include "include/tools/base64.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::tools
{

TEST(Base64Test, test_encode_decode_gives_original_value)
{
    EXPECT_EQ("chicken", Base64Decode(Base64Encode("chicken", 7)));
    EXPECT_EQ("123", Base64Decode(Base64Encode("123", 3)));
    EXPECT_EQ("hello", Base64Decode(Base64Encode("hello", 5)));
    EXPECT_EQ("-_-", Base64Decode(Base64Encode("-_-", 3)));
    EXPECT_EQ("   ", Base64Decode(Base64Encode("   ", 3)));
    EXPECT_EQ("", Base64Decode(Base64Encode("", 0)));
}

TEST(Base64Test, test_encode)
{
    EXPECT_EQ("Z29vc2U=", Base64Encode("goose", 5));
    EXPECT_EQ("IA==", Base64Encode(" ", 1));
    EXPECT_EQ("a2lwcGVy", Base64Encode("kipper", 6));
    EXPECT_EQ("MDA3dHVydGxl", Base64Encode("007turtle", 9));
}

TEST(Base64Test, test_decode)
{
    EXPECT_EQ("mushroom", Base64Decode("bXVzaHJvb20="));
    EXPECT_EQ("===-====-===", Base64Decode("PT09LT09PT0tPT09"));
    EXPECT_EQ("42424242", Base64Decode("NDI0MjQyNDI="));
    EXPECT_EQ(" jellyfish_", Base64Decode("IGplbGx5ZmlzaF8="));
}

}