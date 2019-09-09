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

#include "src/tools/string_char_iterator.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::tools
{

TEST(StringCharIteratorTest, test_reset)
{
    StringCharIterator tok = StringCharIterator("1,2,3", ',');
    tok.Next();
    tok.Next();
    tok.Next();
    ASSERT_FALSE(tok.HasNext());
    tok.Reset();
    ASSERT_TRUE(tok.HasNext());
    ASSERT_STREQ("1", tok.Next().c_str());
}

TEST(StringCharIteratorTest, test_reset_string)
{
    StringCharIterator tok = StringCharIterator("1,2,3", ',');
    tok.Next();
    tok.Reset("new,string,test");
    ASSERT_STREQ("new", tok.Next().c_str());
}

TEST(StringCharIteratorTest, test_has_next)
{
    StringCharIterator tok = StringCharIterator("", ',');
    ASSERT_FALSE(tok.HasNext());
}

TEST(StringCharIteratorTest, test_has_next2)
{
    StringCharIterator tok = StringCharIterator("1,2,3,4", ',');
    ASSERT_TRUE(tok.HasNext());
    tok.Next();
    ASSERT_TRUE(tok.HasNext());
    tok.Next();
    ASSERT_TRUE(tok.HasNext());
    tok.Next();
    ASSERT_TRUE(tok.HasNext());
    tok.Next();
    ASSERT_FALSE(tok.HasNext());
}

TEST(StringCharIteratorTest, test_has_next3)
{
    StringCharIterator tok = StringCharIterator(",", ',');
    ASSERT_TRUE(tok.HasNext());
    tok.Next();
    ASSERT_TRUE(tok.HasNext());
    tok.Next();
    ASSERT_FALSE(tok.HasNext());
}

TEST(StringCharIteratorTest, test_next)
{
    StringCharIterator tok = StringCharIterator(",1,,222,33,,", ',');
    ASSERT_STREQ("", tok.Next().c_str());
    ASSERT_STREQ("1", tok.Next().c_str());
    ASSERT_STREQ("", tok.Next().c_str());
    ASSERT_STREQ("222", tok.Next().c_str());
    ASSERT_STREQ("33", tok.Next().c_str());
    ASSERT_STREQ("", tok.Next().c_str());
    ASSERT_STREQ("", tok.Next().c_str());

    try
    {
        ASSERT_STREQ("", tok.Next().c_str());
        FAIL();
    }
    catch (std::exception &e)
    {
    }
}

TEST(StringCharIteratorTest, test_next1)
{
    StringCharIterator tok = StringCharIterator("bid=22,ask=23,size=99", ',');
    ASSERT_STREQ("bid", tok.Next('=').c_str());
    ASSERT_STREQ("22", tok.Next(',').c_str());
    ASSERT_STREQ("ask", tok.Next('=').c_str());
    ASSERT_STREQ("23", tok.Next(',').c_str());
    ASSERT_STREQ("size", tok.Next('=').c_str());
    ASSERT_STREQ("99", tok.Next(',').c_str());
    ASSERT_FALSE(tok.HasNext());
}

TEST(StringCharIteratorTest, test_next2)
{
    StringCharIterator tok = StringCharIterator(",222", ',');
    ASSERT_STREQ("", tok.Next().c_str());
    ASSERT_STREQ("222", tok.Next().c_str());

    try
    {
        ASSERT_STREQ("", tok.Next().c_str());
        FAIL();
    }
    catch (std::exception& e)
    {
    }
}

TEST(StringCharIteratorTest, test_next3)
{
    StringCharIterator tok = StringCharIterator("111,", ',');
    ASSERT_STREQ("111", tok.Next().c_str());
    ASSERT_STREQ("", tok.Next().c_str());

    try
    {
        ASSERT_STREQ("", tok.Next().c_str());
        FAIL();
    }
    catch (std::exception& e)
    {
    }
}

TEST(StringCharIteratorTest, test_count_tokens)
{
    StringCharIterator tok = StringCharIterator("", '/');
    ASSERT_EQ(0, tok.CountTokens());
}

TEST(StringCharIteratorTest, test_count_tokens1)
{
    StringCharIterator tok = StringCharIterator("aa", '/');
    ASSERT_EQ(1, tok.CountTokens());
}

TEST(StringCharIteratorTest, test_count_tokens2)
{
    StringCharIterator tok = StringCharIterator("aaa/bb", '/');
    ASSERT_EQ(2, tok.CountTokens());
}

TEST(StringCharIteratorTest, test_count_tokens3)
{
    StringCharIterator tok = StringCharIterator("//", '/');
    ASSERT_EQ(3, tok.CountTokens());
}

TEST(StringCharIteratorTest, test_count_tokens4)
{
    StringCharIterator tok = StringCharIterator("1//3/444", '/');
    ASSERT_EQ(4, tok.CountTokens());
}

}
