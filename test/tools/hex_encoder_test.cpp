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

#include <iostream>
#include "include/tools/date.h"
#include "src/tools/hex_encoder.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::tools
{

TEST(HexEncoderTest, test_encode_as_hex)
{
    ASSERT_EQ("", HexEncoder::EncodeAsHex((unsigned char *) "", 0));
    ASSERT_EQ("3a", HexEncoder::EncodeAsHex((unsigned char *) ":", 1));
    ASSERT_EQ("60", HexEncoder::EncodeAsHex((unsigned char *) "`", 1));
    ASSERT_EQ("61", HexEncoder::EncodeAsHex((unsigned char *) "a", 1));
    ASSERT_EQ("5d", HexEncoder::EncodeAsHex((unsigned char *) "]", 1));
    ASSERT_EQ("7b", HexEncoder::EncodeAsHex((unsigned char *) "{", 1));
    ASSERT_EQ("3a60615d7b", HexEncoder::EncodeAsHex((unsigned char *) ":`a]{", 5));
};

TEST(HexEncoderTest, test_encode_as_hex_offset)
{
    ASSERT_EQ("", HexEncoder::EncodeAsHex((unsigned char *) ":`a]{", 0, 0, 5));
    ASSERT_EQ("", HexEncoder::EncodeAsHex((unsigned char *) ":`a]{", 3, 0, 5));
    ASSERT_EQ("5d7b", HexEncoder::EncodeAsHex((unsigned char *) ":`a]{", 3, 5, 5));
    ASSERT_EQ("3a60615d7b", HexEncoder::EncodeAsHex((unsigned char *) ":`a]{", 0, 5, 5));
};

TEST(HexEncoderTest, test_decode_from_hex)
{
    ASSERT_EQ((unsigned char) 35, HexEncoder::DecodeFromHex("23").at(0));
    ASSERT_EQ((unsigned char) 0, HexEncoder::DecodeFromHex("00").at(0));
    ASSERT_EQ((unsigned char) 86, HexEncoder::DecodeFromHex("56").at(0));
    ASSERT_EQ((unsigned char) 105, HexEncoder::DecodeFromHex("69").at(0));
    ASSERT_EQ((unsigned char) 91, HexEncoder::DecodeFromHex("5B").at(0));
    ASSERT_EQ((unsigned char) 121, HexEncoder::DecodeFromHex("79").at(0));
}

TEST(HexEncoderTest, test_decode_from_hex_odd)
{
    ASSERT_EQ((unsigned char) 0, HexEncoder::DecodeFromHex("0").at(0));
    ASSERT_EQ((unsigned char) 1, HexEncoder::DecodeFromHex("17f").at(0));
    ASSERT_EQ((unsigned char) 127, HexEncoder::DecodeFromHex("17f").at(1));
    ASSERT_EQ((unsigned char) 2, HexEncoder::DecodeFromHex("280").at(0));
    ASSERT_EQ((unsigned char) -128, HexEncoder::DecodeFromHex("280").at(1));
}

TEST(HexEncoderTest, test_decode_from_hex_uppercase)
{
    ASSERT_EQ((unsigned char) 1, HexEncoder::DecodeFromHex("17F").at(0));
    ASSERT_EQ((unsigned char) 127, HexEncoder::DecodeFromHex("17F").at(1));

    ASSERT_EQ((unsigned char) 10, HexEncoder::DecodeFromHex("a").at(0));
    ASSERT_EQ((unsigned char) 10, HexEncoder::DecodeFromHex("A").at(0));

    ASSERT_EQ((unsigned char) -1, HexEncoder::DecodeFromHex("ffFF").at(0));
    ASSERT_EQ((unsigned char) -1, HexEncoder::DecodeFromHex("ffFF").at(1));

    ASSERT_EQ((unsigned char) 1, HexEncoder::DecodeFromHex("123FF").at(0));
    ASSERT_EQ((unsigned char) 35, HexEncoder::DecodeFromHex("123FF").at(1));
    ASSERT_EQ((unsigned char) -1, HexEncoder::DecodeFromHex("123FF").at(2));

    ASSERT_EQ((unsigned char) 0, HexEncoder::DecodeFromHex("0010203FFB3423710").at(0));
    ASSERT_EQ((unsigned char) 1, HexEncoder::DecodeFromHex("0010203FFB3423710").at(1));
    ASSERT_EQ((unsigned char) 2, HexEncoder::DecodeFromHex("0010203FFB3423710").at(2));
    ASSERT_EQ((unsigned char) 3, HexEncoder::DecodeFromHex("0010203FFB3423710").at(3));
    ASSERT_EQ((unsigned char) -1, HexEncoder::DecodeFromHex("0010203FFB3423710").at(4));
    ASSERT_EQ((unsigned char) -77, HexEncoder::DecodeFromHex("0010203FFB3423710").at(5));
    ASSERT_EQ((unsigned char) 66, HexEncoder::DecodeFromHex("0010203FFB3423710").at(6));
    ASSERT_EQ((unsigned char) 55, HexEncoder::DecodeFromHex("0010203FFB3423710").at(7));
    ASSERT_EQ((unsigned char) 16, HexEncoder::DecodeFromHex("0010203FFB3423710").at(8));
}

TEST(HexEncoderTest, test_two_way_all_chars)
{
    std::vector<unsigned char> all_chars1 = HexEncoder::DecodeFromHex("0123456789abcdefABCDEF");
    ASSERT_STREQ("0123456789abcdefabcdef", HexEncoder::EncodeAsHex(&all_chars1[0], all_chars1.size()).c_str());

    std::vector<unsigned char> all_chars2 = HexEncoder::DecodeFromHex("abcdef");
    ASSERT_STREQ("abcdef", HexEncoder::EncodeAsHex(&all_chars2[0], all_chars2.size()).c_str());

    std::vector<unsigned char> all_chars3 = HexEncoder::DecodeFromHex("ABCDEF");
    ASSERT_STREQ("abcdef", HexEncoder::EncodeAsHex(&all_chars3[0], all_chars3.size()).c_str());
}

}
