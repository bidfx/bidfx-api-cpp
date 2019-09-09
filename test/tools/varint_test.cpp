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
#include <stdexcept>
#include <cstring>
#include "varint_test.h"
#include "src/tools/varint.h"
#include "gtest/gtest.h"
#include "src/tools/buffer_util.h"

namespace bidfx_public_api::tools
{

using bidfx_public_api::tools::Varint;
using bidfx_public_api::tools::ByteBuffer;

static std::string HexChars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

TEST(VarintTest, TestWriteUnsigned32)
{
    ASSERT_EQ("00", HexEncoderDecoder::WriteU32ToHexString(0, 1));
    ASSERT_EQ("01", HexEncoderDecoder::WriteU32ToHexString(1, 1));
    ASSERT_EQ("02", HexEncoderDecoder::WriteU32ToHexString(2, 1));
    ASSERT_EQ("11", HexEncoderDecoder::WriteU32ToHexString(17, 1));
    ASSERT_EQ("7f", HexEncoderDecoder::WriteU32ToHexString(127, 1));

    ASSERT_EQ("8001", HexEncoderDecoder::WriteU32ToHexString(128, 2));
    ASSERT_EQ("8002", HexEncoderDecoder::WriteU32ToHexString(256, 2));
    ASSERT_EQ("a274", HexEncoderDecoder::WriteU32ToHexString(14882, 2));
    ASSERT_EQ("a08d06", HexEncoderDecoder::WriteU32ToHexString(100000, 3));
    ASSERT_EQ("ffffffff", HexEncoderDecoder::WriteU32ToHexString(2147483647, 4));
}

TEST(VarintTest, TestWriteU32CanBeReadWithReadU32)
{
    ByteBuffer buffer255 = HexEncoderDecoder::WriteU32ToNewStream(255);
    ByteBuffer buffer1 = HexEncoderDecoder::WriteU32ToNewStream(1);
    ByteBuffer buffer2 = HexEncoderDecoder::WriteU32ToNewStream(2);
    ByteBuffer buffer3 = HexEncoderDecoder::WriteU32ToNewStream(3);
    ByteBuffer buffer17 = HexEncoderDecoder::WriteU32ToNewStream(17);
    ByteBuffer buffer127 = HexEncoderDecoder::WriteU32ToNewStream(127);
    ByteBuffer buffer128 = HexEncoderDecoder::WriteU32ToNewStream(128);
    ByteBuffer buffer1000 = HexEncoderDecoder::WriteU32ToNewStream(1000);
    ByteBuffer buffer10000 = HexEncoderDecoder::WriteU32ToNewStream(10000);
    ByteBuffer buffer872682762 = HexEncoderDecoder::WriteU32ToNewStream(872682762);
    ByteBuffer buffer2147483647 = HexEncoderDecoder::WriteU32ToNewStream(2147483647);

    ASSERT_EQ(255, Varint::ReadU32(buffer255));
    ASSERT_EQ(1, Varint::ReadU32(buffer1));
    ASSERT_EQ(2, Varint::ReadU32(buffer2));
    ASSERT_EQ(3, Varint::ReadU32(buffer3));
    ASSERT_EQ(17, Varint::ReadU32(buffer17));
    ASSERT_EQ(127, Varint::ReadU32(buffer127));
    ASSERT_EQ(128, Varint::ReadU32(buffer128));
    ASSERT_EQ(1000, Varint::ReadU32(buffer1000));
    ASSERT_EQ(10000, Varint::ReadU32(buffer10000));
    ASSERT_EQ(872682762, Varint::ReadU32(buffer872682762));
    ASSERT_EQ(2147483647, Varint::ReadU32(buffer2147483647));
}

TEST(VarintTest, TestReadUnsigned32)
{
    ByteBuffer buffer00 = HexEncoderDecoder::HexStream("00");
    ByteBuffer buffer01 = HexEncoderDecoder::HexStream("01");
    ByteBuffer buffer02 = HexEncoderDecoder::HexStream("02");
    ByteBuffer buffer10 = HexEncoderDecoder::HexStream("10");
    ByteBuffer buffer7f = HexEncoderDecoder::HexStream("7f");
    ByteBuffer buffer8001 = HexEncoderDecoder::HexStream("8001");
    ByteBuffer buffer8101 = HexEncoderDecoder::HexStream("8101");
    ByteBuffer buffer8201 = HexEncoderDecoder::HexStream("8201");
    ByteBuffer bufferffff = HexEncoderDecoder::HexStream("ffff03");

    EXPECT_EQ(0, Varint::ReadU32(buffer00));
    EXPECT_EQ(1, Varint::ReadU32(buffer01));
    EXPECT_EQ(2, Varint::ReadU32(buffer02));
    EXPECT_EQ(16, Varint::ReadU32(buffer10));
    EXPECT_EQ(127, Varint::ReadU32(buffer7f));

    EXPECT_EQ(128, Varint::ReadU32(buffer8001));
    EXPECT_EQ(129, Varint::ReadU32(buffer8101));
    EXPECT_EQ(130, Varint::ReadU32(buffer8201));
    EXPECT_EQ(65535, Varint::ReadU32(bufferffff));
}

TEST(VarintTest, TestWriteUnsigned64)
{
    ASSERT_EQ("00", HexEncoderDecoder::WriteU64ToHexString(0, 1));
    ASSERT_EQ("01", HexEncoderDecoder::WriteU64ToHexString(1, 1));
    ASSERT_EQ("02", HexEncoderDecoder::WriteU64ToHexString(2, 1));
    ASSERT_EQ("11", HexEncoderDecoder::WriteU64ToHexString(17, 1));
    ASSERT_EQ("7f", HexEncoderDecoder::WriteU64ToHexString(127, 1));

    ASSERT_EQ("8001", HexEncoderDecoder::WriteU64ToHexString(128, 2));
    ASSERT_EQ("a274", HexEncoderDecoder::WriteU64ToHexString(14882, 2));
    ASSERT_EQ("a08d06", HexEncoderDecoder::WriteU64ToHexString(100000, 3));
    ASSERT_EQ("ffffffffffffffff",
              HexEncoderDecoder::WriteU64ToHexString(9223372036854775807L, 8)); // Endianness might affect his test!!
}

TEST(VarintTest, TestWriteU64WithNegativeUsesUpTenBytes)
{
    ByteBuffer buffer = ByteBuffer(16);
    Varint::WriteU64(buffer, static_cast<uint64_t>(-23L));
    ASSERT_EQ(10, buffer.ReadableBytes());
}

TEST(VarintTest, TestWriteU64CanBeReadWithReadU64)
{
    ByteBuffer buffer0 = HexEncoderDecoder::WriteU64ToNewStream(0);
    ByteBuffer buffer1 = HexEncoderDecoder::WriteU64ToNewStream(1);
    ByteBuffer buffer2 = HexEncoderDecoder::WriteU64ToNewStream(2);
    ByteBuffer buffer3 = HexEncoderDecoder::WriteU64ToNewStream(3);
    ByteBuffer buffer17 = HexEncoderDecoder::WriteU64ToNewStream(17);
    ByteBuffer buffer127 = HexEncoderDecoder::WriteU64ToNewStream(127);
    ByteBuffer buffer128 = HexEncoderDecoder::WriteU64ToNewStream(128);
    ByteBuffer buffer1000 = HexEncoderDecoder::WriteU64ToNewStream(1000);
    ByteBuffer buffer10000 = HexEncoderDecoder::WriteU64ToNewStream(10000);
    ByteBuffer buffer872682762 = HexEncoderDecoder::WriteU64ToNewStream(872682762);
    ByteBuffer buffer2147483647 = HexEncoderDecoder::WriteU64ToNewStream(2147483647);
    ByteBuffer buffer10000000000000L = HexEncoderDecoder::WriteU64ToNewStream(10000000000000L);
    ByteBuffer buffer836286376726767676L = HexEncoderDecoder::WriteU64ToNewStream(836286376726767676L);
    ByteBuffer buffer_max = HexEncoderDecoder::WriteU64ToNewStream(UINT64_MAX);

    ASSERT_EQ(0, Varint::ReadU64(buffer0));
    ASSERT_EQ(1, Varint::ReadU64(buffer1));
    ASSERT_EQ(2, Varint::ReadU64(buffer2));
    ASSERT_EQ(3, Varint::ReadU64(buffer3));
    ASSERT_EQ(17, Varint::ReadU64(buffer17));
    ASSERT_EQ(127, Varint::ReadU64(buffer127));
    ASSERT_EQ(128, Varint::ReadU64(buffer128));
    ASSERT_EQ(1000, Varint::ReadU64(buffer1000));
    ASSERT_EQ(10000, Varint::ReadU64(buffer10000));
    ASSERT_EQ(872682762, Varint::ReadU64(buffer872682762));
    ASSERT_EQ(2147483647, Varint::ReadU64(buffer2147483647));
    ASSERT_EQ(10000000000000L, Varint::ReadU64(buffer10000000000000L));
    ASSERT_EQ(836286376726767676L, Varint::ReadU64(buffer836286376726767676L));
    ASSERT_EQ(UINT64_MAX, Varint::ReadU64(buffer_max));
}

TEST(VarintTest, TestReadUnsigned64)
{
    ByteBuffer buffer00 = HexEncoderDecoder::HexStream("00");
    ByteBuffer buffer01 = HexEncoderDecoder::HexStream("01");
    ByteBuffer buffer02 = HexEncoderDecoder::HexStream("02");
    ByteBuffer buffer16 = HexEncoderDecoder::HexStream("10");
    ByteBuffer buffer127 = HexEncoderDecoder::HexStream("7f");
    ByteBuffer buffer128 = HexEncoderDecoder::HexStream("8001");
    ByteBuffer buffer129 = HexEncoderDecoder::HexStream("8101");
    ByteBuffer buffer130 = HexEncoderDecoder::HexStream("8201");
    ByteBuffer buffer65535 = HexEncoderDecoder::HexStream("ffff03");

    ASSERT_EQ(0, Varint::ReadU64(buffer00));
    ASSERT_EQ(1, Varint::ReadU64(buffer01));
    ASSERT_EQ(2, Varint::ReadU64(buffer02));
    ASSERT_EQ(16, Varint::ReadU64(buffer16));
    ASSERT_EQ(127, Varint::ReadU64(buffer127));

    ASSERT_EQ(128, Varint::ReadU64(buffer128));
    ASSERT_EQ(129, Varint::ReadU64(buffer129));
    ASSERT_EQ(130, Varint::ReadU64(buffer130));
    ASSERT_EQ(65535, Varint::ReadU64(buffer65535)); // Endianness might affect his test!!
}

TEST(VarintTest, TestReadString)
{
    ByteBuffer buffer_null = BufferUtil::StringAsBuffer(std::string("\000", 1));
    ByteBuffer buffer_empty = BufferUtil::StringAsBuffer("\001");
    ByteBuffer buffer_X = BufferUtil::StringAsBuffer("\002X");
    ByteBuffer buffer_test = BufferUtil::StringAsBuffer("\005test");
    ByteBuffer buffer_hello_world = BufferUtil::StringAsBuffer("\014hello world");

    EXPECT_EQ(Varint::kNullString, Varint::ReadString(buffer_null));
    EXPECT_EQ("", Varint::ReadString(buffer_empty));
    EXPECT_EQ("X", Varint::ReadString(buffer_X));
    EXPECT_EQ("test", Varint::ReadString(buffer_test));
    EXPECT_EQ("hello world", Varint::ReadString(buffer_hello_world));
}

TEST(VarintTest, TestWriteNullString)
{
    ByteBuffer buffer = ByteBuffer(10);
    Varint::WriteString(buffer, std::string("\000", 1));
    EXPECT_EQ(std::string("\000", 1), buffer.ToString());
}

TEST(VarintTest, TestWriteNullConstant)
{
    ByteBuffer buffer = ByteBuffer(10);
    Varint::WriteString(buffer, Varint::kNullString);
    EXPECT_EQ(std::string("\000", 1), buffer.ToString());
}

TEST(VarintTest, TestWriteEmptyString)
{
    ByteBuffer buffer = ByteBuffer(10);
    Varint::WriteString(buffer, "");
    EXPECT_EQ("\001", buffer.ToString());
}

TEST(VarintTest, TestWriteSingleCharacter)
{
    ByteBuffer buffer = ByteBuffer(10);
    Varint::WriteString(buffer, "X");
    EXPECT_EQ("\002X", buffer.ToString());
}

TEST(VarintTest, TestWriteString)
{
    ByteBuffer buffer = ByteBuffer(10);
    Varint::WriteString(buffer, "test");
    EXPECT_EQ("\05test", buffer.ToString());
}

TEST(VarintTest, TestZigzagToInt)
{
    ASSERT_EQ(0, Varint::ZigzagToInt(0));
    ASSERT_EQ(-1, Varint::ZigzagToInt(1));
    ASSERT_EQ(1, Varint::ZigzagToInt(2));
    ASSERT_EQ(-2, Varint::ZigzagToInt(3));
    ASSERT_EQ(2, Varint::ZigzagToInt(4));
    ASSERT_EQ(-3, Varint::ZigzagToInt(5));
    ASSERT_EQ(3, Varint::ZigzagToInt(6));
    ASSERT_EQ(-4, Varint::ZigzagToInt(7));
    ASSERT_EQ(4, Varint::ZigzagToInt(8));

    ASSERT_EQ(1000, Varint::ZigzagToInt(2000));
    ASSERT_EQ(-1000, Varint::ZigzagToInt(1999));
}

TEST(VarintTest, TestZigzagToLong)
{
    ASSERT_EQ(0, Varint::ZigzagToLong(0));
    ASSERT_EQ(-1, Varint::ZigzagToLong(1));
    ASSERT_EQ(1, Varint::ZigzagToLong(2));
    ASSERT_EQ(-2, Varint::ZigzagToLong(3));
    ASSERT_EQ(2, Varint::ZigzagToLong(4));
    ASSERT_EQ(-3, Varint::ZigzagToLong(5));
    ASSERT_EQ(3, Varint::ZigzagToLong(6));
    ASSERT_EQ(-4, Varint::ZigzagToLong(7));
    ASSERT_EQ(4, Varint::ZigzagToLong(8));

    ASSERT_EQ(10000000000L, Varint::ZigzagToLong(20000000000L));
    ASSERT_EQ(-10000000000L, Varint::ZigzagToLong(19999999999L));
}

TEST(VarintTest, TestWriteEmptyStringVector)
{
    ByteBuffer buffer = ByteBuffer();
    std::vector<std::string> strings;
    Varint::WriteStringVector(buffer, strings);
    EXPECT_EQ(std::string("\000", 1), buffer.ToString());
}

TEST(VarintTest, TestWriteOneElementStringVector)
{
    ByteBuffer buffer = ByteBuffer();
    std::vector<std::string> strings;
    strings.emplace_back("test");
    Varint::WriteStringVector(buffer, strings);
    EXPECT_EQ("\001\005test", buffer.ToString());
}

TEST(VarintTest, TestWriteTwoElementStringVector)
{
    ByteBuffer buffer = ByteBuffer();
    std::vector<std::string> strings;
    strings.emplace_back("one");
    strings.emplace_back("two");
    Varint::WriteStringVector(buffer, strings);
    EXPECT_EQ("\002\004one\004two", buffer.ToString());
}

TEST(VarintTest, TestVectorWrittenThenReadIsEqualToOriginal)
{
    ByteBuffer buffer = ByteBuffer();
    std::vector<std::string> strings;
    strings.emplace_back("one");
    strings.emplace_back("two");
    Varint::WriteStringVector(buffer, strings);
    EXPECT_EQ(strings, Varint::ReadStringVector(buffer));
}

ByteBuffer HexEncoderDecoder::WriteStringToStream(std::string s)
{
    size_t string_len = s.length();
    uint8_t* bytes = new uint8_t[string_len];
    memcpy(bytes, &s[0], string_len);
    ByteBuffer buffer = ByteBuffer(string_len + 1);
    buffer.WriteByte(static_cast<unsigned char>(string_len));
    buffer.WriteBytes(bytes, static_cast<uint32_t>(string_len));
    delete (bytes);
    return buffer;
}

std::string HexEncoderDecoder::WriteU32ToHexString(int value, uint32_t size)
{
    ByteBuffer buffer = HexEncoderDecoder::WriteU32ToNewStream(value);
    return HexEncoderDecoder::StreamAsHex(buffer, size);
}

std::string HexEncoderDecoder::WriteU64ToHexString(long value, uint32_t size)
{
    ByteBuffer buffer = HexEncoderDecoder::WriteU64ToNewStream(value);
    return HexEncoderDecoder::StreamAsHex(buffer, size);
}

ByteBuffer HexEncoderDecoder::WriteU32ToNewStream(int value)
{
    ByteBuffer buffer = ByteBuffer(12);
    Varint::WriteU32(buffer, value);
    return buffer;
}

ByteBuffer HexEncoderDecoder::WriteU64ToNewStream(long value)
{
    ByteBuffer buffer = ByteBuffer(12);
    Varint::WriteU64(buffer, value);
    return buffer;
}

std::string HexEncoderDecoder::StreamAsHex(ByteBuffer& buffer, uint32_t size)
{
    uint8_t* arr = buffer.ToArray();
    return HexEncoderDecoder::EncodeAsHex(arr, size);
}

ByteBuffer HexEncoderDecoder::HexStream(std::string hex_string)
{
    uint8_t* bytes = HexEncoderDecoder::DecodeFromHex(hex_string);
    ByteBuffer buffer = ByteBuffer(12);
    buffer.WriteBytes(bytes, hex_string.length() + 1);
    delete(bytes);
    return buffer;
}

std::string HexEncoderDecoder::EncodeAsHex(uint8_t* bytes, uint32_t size)
{
    std::string s = std::string();
    for (int i = 0; i < size; i++)
    {
        s += HexChars[(bytes[i] >> 4) & 0xf];
        s += HexChars[(bytes[i]) & 0xf];
    }
    delete(bytes);
    return s;
}

uint8_t* HexEncoderDecoder::DecodeFromHex(std::string hex)
{
    uint8_t* bytes = new uint8_t[(hex.length()) >> 1];

    int byte_index = 0;
    int char_index = 0;

    if ((hex.length() & 1) == 1)
    {
        bytes[byte_index++] = (uint8_t) HexEncoderDecoder::DecodeNibble(hex[char_index++]);
    }

    while (byte_index < (hex.length()) >> 1)
    {
        char a = hex[char_index++];
        char b = hex[char_index++];
        bytes[byte_index++] = HexEncoderDecoder::DecodeByte(a, b);
    }
    return bytes;
}

int HexEncoderDecoder::DecodeNibble(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }

    if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }

    if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    throw std::invalid_argument("NEEDS TO BE VALID HEXADECIMAL");
}

uint8_t HexEncoderDecoder::DecodeByte(char c2, char c1)
{
    int low = DecodeNibble(c1);
    int high = DecodeNibble(c2);
    return (uint8_t) (((high & 0x0f) << 4) | low);
}

}