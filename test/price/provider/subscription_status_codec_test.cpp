/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */
 
#include "gtest/gtest.h"
#include "src/price/provider/subscription_status_codec.h"
#include "include/subscription_status.h"

namespace bidfx_public_api::provider
{
class SubscriptionStatusCodecTest : public testing::Test
{
};

TEST_F(SubscriptionStatusCodecTest, TestDecodeStatus)
{
    EXPECT_EQ(SubscriptionStatus::OK, SubscriptionStatusCodec::DecodeStatus('O'));
    EXPECT_EQ(SubscriptionStatus::PENDING, SubscriptionStatusCodec::DecodeStatus('P'));
    EXPECT_EQ(SubscriptionStatus::STALE, SubscriptionStatusCodec::DecodeStatus('S'));
    EXPECT_EQ(SubscriptionStatus::CANCELLED, SubscriptionStatusCodec::DecodeStatus('C'));
    EXPECT_EQ(SubscriptionStatus::DISCONTINUED, SubscriptionStatusCodec::DecodeStatus('D'));
    EXPECT_EQ(SubscriptionStatus::PROHIBITED, SubscriptionStatusCodec::DecodeStatus('H'));
    EXPECT_EQ(SubscriptionStatus::UNAVAILABLE, SubscriptionStatusCodec::DecodeStatus('U'));
    EXPECT_EQ(SubscriptionStatus::REJECTED, SubscriptionStatusCodec::DecodeStatus('R'));
    EXPECT_EQ(SubscriptionStatus::TIMEOUT, SubscriptionStatusCodec::DecodeStatus('T'));
    EXPECT_EQ(SubscriptionStatus::INACTIVE, SubscriptionStatusCodec::DecodeStatus('I'));
    EXPECT_EQ(SubscriptionStatus::EXHAUSTED, SubscriptionStatusCodec::DecodeStatus('E'));
    EXPECT_EQ(SubscriptionStatus::CLOSED, SubscriptionStatusCodec::DecodeStatus('L'));
}

TEST_F(SubscriptionStatusCodecTest, TestDecodingOfUnknownEncodingReturnsPending)
{
    EXPECT_EQ(SubscriptionStatus::PENDING, SubscriptionStatusCodec::DecodeStatus(3));
    EXPECT_EQ(SubscriptionStatus::PENDING, SubscriptionStatusCodec::DecodeStatus(127));
    EXPECT_EQ(SubscriptionStatus::PENDING, SubscriptionStatusCodec::DecodeStatus('1'));
}

}