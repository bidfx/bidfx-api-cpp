/**
 * Copyright 2019 BidFX Systems Ltd. All rights reserved.
 */

#include "gtest/gtest.h"
#include "src/tools/guid.h"

namespace bidfx_public_api::tools
{
class GUIDTest : public ::testing::Test
{
};

TEST_F(GUIDTest, test_string_length_is_32)
{
    for (int it = 0; it < 1000; it++)
    {
        GUID guid;
        ASSERT_EQ(32, guid.ToString().size());
    }
}

TEST_F(GUIDTest, test_new_guid_differ)
{
    for (int it = 0; it < 1000; it++)
    {
        GUID guid0;
        GUID guid1;
        GUID guid2;
        GUID guid3;
        GUID guid4;
        GUID guid5;
        ASSERT_NE(guid0.ToString(), guid1.ToString());
        ASSERT_NE(guid0.ToString(), guid2.ToString());
        ASSERT_NE(guid0.ToString(), guid3.ToString());
        ASSERT_NE(guid0.ToString(), guid4.ToString());
        ASSERT_NE(guid0.ToString(), guid5.ToString());
    }
}

TEST_F(GUIDTest, test_guid_string_is_hex)
{
    for (int it = 0; it < 1000; it++)
    {
        GUID guid;
        std::string string_val = guid.ToString();
        for (int i = 0; i < string_val.size(); i++)
        {
            ASSERT_TRUE(string_val[i] >= '0' && string_val[i] <= '9' ||
                        string_val[i] >= 'A' && string_val[i] <= 'F' ||
                        string_val[i] >= 'a' && string_val[i] <= 'f');
        }
    }
}
}