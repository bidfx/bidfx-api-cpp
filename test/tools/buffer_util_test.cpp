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

#include "src/tools/buffer_util.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::tools
{

TEST(BufferUtilTest, test_hex_buffer_with_empty_string_create_an_empty_buffer)
{
    ASSERT_EQ(0, BufferUtil::HexBuffer("").ReadableBytes());
}

TEST(BufferUtilTest, test_hex_buffer_with_hex_codes)
{
    ByteBuffer buffer = BufferUtil::HexBuffer("0201");
    unsigned char bytes[2];
    buffer.ReadBytes(bytes, 2);

    ASSERT_EQ('\x02', bytes[0]);
    ASSERT_EQ('\x01', bytes[1]);
}

TEST(BufferUtilTest, test_buffer_as_hex_turns_empty_buffer_into_empty_string)
{
    ByteBuffer empty_buffer(0);
    ASSERT_EQ("", BufferUtil::BufferAsHex(empty_buffer));
}

TEST(BufferUtilTest, test_buffer_as_hex)
{
    ByteBuffer buffer1(1);
    unsigned char bytes1[1] = {0};
    buffer1.WriteBytes(bytes1, 1);
    ASSERT_EQ("00", BufferUtil::BufferAsHex(buffer1));

    ByteBuffer buffer2(4);
    unsigned char bytes2[4] = {0, 1, 3, 4};
    buffer2.WriteBytes(bytes2, 4);
    ASSERT_EQ("00010304", BufferUtil::BufferAsHex(buffer2));

    ByteBuffer buffer3(2);
    unsigned char bytes3[4] = {16, 255};
    buffer3.WriteBytes(bytes3, 2);
    ASSERT_EQ("10ff", BufferUtil::BufferAsHex(buffer3));
}

TEST(BufferUtilTest, test_empty_buffer_as_string)
{
    ByteBuffer empty_buffer(0);
    ASSERT_EQ("", BufferUtil::BufferAsString(empty_buffer));
}

TEST(BufferUtilTest, test_buffer_as_string)
{
    ByteBuffer buffer1(5);
    unsigned char buffer1_data[5] = {'h', 'e', 'l', 'l', 'o'};
    buffer1.WriteBytes(buffer1_data, 5);
    ASSERT_EQ("hello", BufferUtil::BufferAsString(buffer1));

    ByteBuffer buffer2(3);
    unsigned char buffer2_data[3] = {'1', '2', '3'};
    buffer2.WriteBytes(buffer2_data, 3);
    ASSERT_EQ("123", BufferUtil::BufferAsString(buffer2));

    ByteBuffer buffer3(6);
    unsigned char buffer3_data[6] = {'p', 'i', 'n', 'g', '\r', '\n'};
    buffer3.WriteBytes(buffer3_data, 6);
    ASSERT_EQ("ping\r\n", BufferUtil::BufferAsString(buffer3));
}

}