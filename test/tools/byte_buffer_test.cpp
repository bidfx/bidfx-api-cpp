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
#include "gtest/gtest.h"
#include "src/tools/byte_buffer.h"

namespace bidfx_public_api::tools
{

using bidfx_public_api::tools::ByteBuffer;

TEST(ByteBufferTest, test_one_insertion)
{
    ByteBuffer byte_buffer(12);
    byte_buffer.WriteByte('a');
    EXPECT_EQ('a', byte_buffer.GetNextByte());
}

TEST(ByteBufferTest, test_two_insertion)
{
    ByteBuffer byte_buffer(12);
    byte_buffer.WriteByte('a');
    byte_buffer.WriteByte('b');
    EXPECT_EQ('a', byte_buffer.GetNextByte());
    EXPECT_EQ('b', byte_buffer.GetNextByte());
}

TEST(ByteBufferTest, test_multi_insertion)
{
    ByteBuffer byte_buffer(12);
    std::string input = "abc";
    byte_buffer.WriteBytes((unsigned char *) &input[0], 3);
    EXPECT_EQ('a', byte_buffer.GetNextByte());
    EXPECT_EQ('b', byte_buffer.GetNextByte());
    EXPECT_EQ('c', byte_buffer.GetNextByte());
}


TEST(ByteBufferTest, test_multi_getBytes)
{
    ByteBuffer byte_buffer = ByteBuffer(12);
    std::string input = "abc";
    byte_buffer.WriteBytes((uint8_t*) &input[0], 3);
    char output[3];
    byte_buffer.ReadBytes((uint8_t*) output, 3);
    std::string result(output, 3);

    ASSERT_EQ("abc", result);
}

TEST(ByteBufferTest, test_to_string)
{
    ByteBuffer byte_buffer(20);
    byte_buffer.WriteString("test123");
    byte_buffer.ReadByte();
    EXPECT_EQ("est123", byte_buffer.ToString());
}

TEST(ByteBufferTest, test_fill_buffer_fully_then_insert_and_self_expand)
{
    ByteBuffer byte_buffer(4);
    std::string input = "abcd";
    byte_buffer.WriteBytes((unsigned char *) &input[0], 4);
    ASSERT_EQ(4, byte_buffer.ReadableBytes());
    byte_buffer.WriteBytes((unsigned char *) &input[0], 4);
    EXPECT_EQ(8, byte_buffer.ReadableBytes());
}

TEST(ByteBufferTest, test_buffer_self_expands_when_inserted_bytes_larger_than_remaining_buffer_size)
{
    ByteBuffer byte_buffer(4);
    std::string input = "abcdefgh";
    byte_buffer.WriteBytes((unsigned char *) &input[0], 8);
    EXPECT_EQ(8, byte_buffer.ReadableBytes());
}


TEST(ByteBufferTest, test_fill)
{
    ByteBuffer byte_buffer(12);
    std::string input = "abc";
    byte_buffer.WriteBytes((unsigned char *) &input[0], 3);
    EXPECT_EQ('a', byte_buffer.GetNextByte());
    EXPECT_EQ('b', byte_buffer.GetNextByte());
    EXPECT_EQ('c', byte_buffer.GetNextByte());
    byte_buffer.Fill(0);
    EXPECT_EQ(0, byte_buffer.GetNextByte());
    EXPECT_EQ(0, byte_buffer.GetNextByte());
    EXPECT_EQ(0, byte_buffer.GetNextByte());
}

TEST(ByteBufferTest, test_compact)
{
    ByteBuffer byte_buffer(8);
    std::string input = "abcdefgh";
    byte_buffer.WriteBytes((unsigned char *) &input[0], 8);
    byte_buffer.GetNextByte();
    byte_buffer.GetNextByte();
    byte_buffer.GetNextByte();

    std::string input2 = "ijk";
    byte_buffer.WriteBytes((unsigned char *) &input2[0], 3);
    char output[8];
    byte_buffer.ReadBytes((unsigned char*) &output[0], 8);

    EXPECT_EQ("defghijk", std::string(output, 8));
}

TEST(ByteBufferTest, test_read_int)
{
    ByteBuffer byte_buffer1(4);
    byte_buffer1.WriteInt(123456);
    EXPECT_EQ(123456, byte_buffer1.ReadInt());

    ByteBuffer byte_buffer2(4);
    byte_buffer2.WriteInt(-123456);
    EXPECT_EQ(-123456, byte_buffer2.ReadInt());
}

TEST(ByteBufferTest, test_read_write_long)
{
    int64_t long1 = 56203443221;
    ByteBuffer byte_buffer1(8);
    byte_buffer1.WriteLong(long1);
    EXPECT_EQ(long1, byte_buffer1.ReadLong());

    int64_t long2 = -11234837293;
    ByteBuffer byte_buffer2(8);
    byte_buffer2.WriteLong(long2);
    EXPECT_EQ(long2, byte_buffer2.ReadLong());
}

TEST(ByteBufferTest, test_read_write_short)
{
    int16_t short1 = 566;
    ByteBuffer byte_buffer1(2);
    byte_buffer1.WriteShort(short1);
    EXPECT_EQ(short1, byte_buffer1.ReadShort());

    int16_t short2 = -433;
    ByteBuffer byte_buffer2(2);
    byte_buffer2.WriteShort(short2);
    EXPECT_EQ(short2, byte_buffer2.ReadShort());
}

TEST(ByteBufferTest, test_read_write_medium)
{
    int32_t medium1 = 1048334;
    ByteBuffer byte_buffer1(2);
    byte_buffer1.WriteMedium(medium1);
    EXPECT_EQ(medium1, byte_buffer1.ReadMedium());

    int32_t medium2 = -1048334;
    ByteBuffer byte_buffer2(2);
    byte_buffer2.WriteMedium(medium2);
    EXPECT_EQ(medium2, byte_buffer2.ReadMedium());
}

TEST(ByteBufferTest, test_read_write_float)
{
    float float1 = 4334.32f;
    ByteBuffer byte_buffer1(4);
    byte_buffer1.WriteFloat(float1);
    EXPECT_EQ(float1, byte_buffer1.ReadFloat());

    float float2 = -435.32f;
    ByteBuffer byte_buffer2(4);
    byte_buffer2.WriteFloat(float2);
    EXPECT_EQ(float2, byte_buffer2.ReadFloat());
}

TEST(ByteBufferTEst, test_read_write_double)
{
    double double1 = 583824.324;
    ByteBuffer byte_buffer1(8);
    byte_buffer1.WriteDouble(double1);
    EXPECT_EQ(double1, byte_buffer1.ReadDouble());

    double double2 = -478324.423;
    ByteBuffer byte_buffer2(4);
    byte_buffer2.WriteDouble(double2);
    EXPECT_EQ(double2, byte_buffer2.ReadDouble());
}

TEST(ByteBufferTest, test_set_byte)
{
    ByteBuffer buff(5);
    buff.SetByte(0, 5);
    buff.SetByte(1, 3);
    buff.SetByte(2, 1);
    buff.SetByte(3, 7);
    buff.SetByte(4, 9);
    EXPECT_EQ(5, buff.ReadByte());
    EXPECT_EQ(3, buff.ReadByte());
    EXPECT_EQ(1, buff.ReadByte());
    EXPECT_EQ(7, buff.ReadByte());
    EXPECT_EQ(9, buff.ReadByte());
}

TEST(ByteBufferTest, test_copy_constructor_equals_self)
{
    ByteBuffer a(5);
    a.WriteByte(5);
    a.WriteByte(7);
    a.WriteByte(9);
    a.WriteByte(11);
    a.ReadByte(); //Shift the read index away from 0
    ByteBuffer b(a);
    EXPECT_EQ(a, b);
    EXPECT_EQ(3, b.ReadableBytes());
    EXPECT_EQ(7, b.ReadByte());
}

TEST(ByteBufferTest, test_equals_similar)
{
    ByteBuffer a(5);
    a.WriteByte(5);
    a.WriteByte(7);
    a.WriteByte(9);
    a.WriteByte(11);
    a.ReadByte();
    ByteBuffer b(7);
    b.WriteByte(7);
    b.WriteByte(9);
    b.WriteByte(11);
    EXPECT_EQ(a, b);
}

TEST(ByteBufferTest, test_not_equals_different)
{
    ByteBuffer a(2);
    a.WriteByte(2);
    a.WriteByte(3);
    ByteBuffer b(2);
    b.WriteByte(2);
    b.WriteByte(6);
    EXPECT_NE(a, b);
}
}