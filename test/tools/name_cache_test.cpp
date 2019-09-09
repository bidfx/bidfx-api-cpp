/**
 * Copyright 2019 BidFX Systems Ltd. All rights reserved.
 */

#include <memory>
#include "gtest/gtest.h"
#include "src/tools/name_cache.h"

namespace bidfx_public_api::tools
{
class NameCacheTests : public ::testing::Test
{
public:
    NameCache name_cache_;
};

TEST_F(NameCacheTests, test_create_unique_name)
{
    EXPECT_EQ("Test1", name_cache_.CreateUniqueName("Test"));
    EXPECT_EQ("Test2", name_cache_.CreateUniqueName("Test"));
    EXPECT_EQ("Test3", name_cache_.CreateUniqueName("Test"));
    EXPECT_EQ("Client1", name_cache_.CreateUniqueName("Client"));
    EXPECT_EQ("File.1", name_cache_.CreateUniqueName("File."));
    EXPECT_EQ("File.2", name_cache_.CreateUniqueName("File."));
    EXPECT_EQ("File.3", name_cache_.CreateUniqueName("File."));
}

TEST_F(NameCacheTests, test_next_id)
{
    EXPECT_EQ(1, name_cache_.NextID("Test"));
    EXPECT_EQ(2, name_cache_.NextID("Test"));
    EXPECT_EQ(3, name_cache_.NextID("Test"));
    EXPECT_EQ(1, name_cache_.NextID("Test2"));
}

TEST_F(NameCacheTests, test_last_id)
{
    EXPECT_EQ(0, name_cache_.LastID("Test"));
    EXPECT_EQ(0, name_cache_.LastID("Test"));
    EXPECT_EQ(1, name_cache_.NextID("Test"));
    EXPECT_EQ(2, name_cache_.NextID("Test"));
    EXPECT_EQ(2, name_cache_.LastID("Test"));
    EXPECT_EQ(2, name_cache_.LastID("Test"));
    EXPECT_EQ(0, name_cache_.LastID("Test2"));
    EXPECT_EQ(0, name_cache_.LastID("Test2"));
}

TEST_F(NameCacheTests, test_get_default)
{
    NameCache& cache1 = NameCache::GetDefault();
    NameCache& cache2 = NameCache::GetDefault();
    EXPECT_EQ(&cache1, &cache2);
}

TEST_F(NameCacheTests, test_names)
{
    name_cache_.NextID("Test");
    name_cache_.NextID("Test2");
    name_cache_.NextID("Test2");
    name_cache_.LastID("Test3");
    name_cache_.NextID("Test");
    std::set<std::string> expected = {"Test", "Test2"};
    EXPECT_EQ(expected, name_cache_.Names());
}
}