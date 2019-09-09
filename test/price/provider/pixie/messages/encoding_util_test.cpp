/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include "src/price/provider/pixie/messages/encoding_util.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

class EncodingUtilTest : public ::testing::Test
{
public:
    const double DELTA = 1E-15;
};

TEST_F(EncodingUtilTest, TestDecodeDecimal)
{
    EXPECT_NEAR(1234567890.0, EncodingUtil::DecodeDecimal(1234567890L, 0), DELTA);
    EXPECT_NEAR(1234567890.125, EncodingUtil::DecodeDecimal(1234567890125L, 3), DELTA);
    EXPECT_NEAR(1234567890.123456, EncodingUtil::DecodeDecimal(1234567890123456L, 6), DELTA);
    EXPECT_NEAR(1234567890.25, EncodingUtil::DecodeDecimal(1234567890250000L, 6), DELTA);
}

TEST_F(EncodingUtilTest, TestDecodeBugsSeenInPROD)
{
    // GCS-100613
    int32_t scale = 8;
    EXPECT_NEAR(64.391, EncodingUtil::DecodeDecimal(6439100000L, scale), DELTA);
    EXPECT_NEAR(6.439, EncodingUtil::DecodeDecimal(643900000L, scale), DELTA);
    EXPECT_NEAR(64.1, EncodingUtil::DecodeDecimal(6410000000L, scale), DELTA);
    EXPECT_NEAR(64.99999999, EncodingUtil::DecodeDecimal(6499999999L, scale), DELTA);
    EXPECT_NEAR(64.00000001, EncodingUtil::DecodeDecimal(6400000001L, scale), DELTA);

    // GCS-100797
    EXPECT_NEAR(1.45622, EncodingUtil::DecodeDecimal(145622000L, scale), DELTA);
}

TEST_F(EncodingUtilTest, TestDecodeNegativePrices)
{
    int32_t scale = 8;
    EXPECT_NEAR(-64.391, EncodingUtil::DecodeDecimal(-6439100000L, scale), DELTA);
    EXPECT_NEAR(-6.439, EncodingUtil::DecodeDecimal(-643900000L, scale), DELTA);
    EXPECT_NEAR(-64.1, EncodingUtil::DecodeDecimal(-6410000000L, scale), DELTA);
    EXPECT_NEAR(-64.99999999, EncodingUtil::DecodeDecimal(-6499999999L, scale), DELTA);
    EXPECT_NEAR(-64.00000001, EncodingUtil::DecodeDecimal(-6400000001L, scale), DELTA);
    EXPECT_NEAR(-1.45622, EncodingUtil::DecodeDecimal(-145622000L, scale), DELTA);
}

TEST_F(EncodingUtilTest, TestPowerOfTen)
{
    EXPECT_EQ(1L, EncodingUtil::PowerOfTen(0));
    EXPECT_EQ(10L, EncodingUtil::PowerOfTen(1));
    EXPECT_EQ(100L, EncodingUtil::PowerOfTen(2));
    EXPECT_EQ(1000L, EncodingUtil::PowerOfTen(3));
    EXPECT_EQ(1000000L, EncodingUtil::PowerOfTen(6));
    EXPECT_EQ(1000000000L, EncodingUtil::PowerOfTen(9));
}

TEST_F(EncodingUtilTest, TestCanNotCreateAPowerOfTenBiggerThanJavaMaxInt)
{
    EXPECT_THROW(EncodingUtil::PowerOfTen(20), std::invalid_argument);
}

}