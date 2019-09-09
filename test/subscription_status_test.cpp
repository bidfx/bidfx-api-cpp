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

#include "include/subscription_status.h"
#include "gtest/gtest.h"

namespace bidfx_public_api
{

using bidfx_public_api::subscription_status_map;

TEST(SubscriptonStatusTest, test_to_string)
{
    EXPECT_EQ("UNAVAILABLE", subscription_status_map[SubscriptionStatus::UNAVAILABLE]);
    EXPECT_EQ("CLOSED", subscription_status_map[SubscriptionStatus::CLOSED]);
    EXPECT_EQ("CANCELLED", subscription_status_map[SubscriptionStatus::CANCELLED]);
    EXPECT_EQ("DISCONTINUED", subscription_status_map[SubscriptionStatus::DISCONTINUED]);
    EXPECT_EQ("EXHAUSTED", subscription_status_map[SubscriptionStatus::EXHAUSTED]);
    EXPECT_EQ("INACTIVE", subscription_status_map[SubscriptionStatus::INACTIVE]);
    EXPECT_EQ("OK", subscription_status_map[SubscriptionStatus::OK]);
    EXPECT_EQ("PENDING", subscription_status_map[SubscriptionStatus::PENDING]);
    EXPECT_EQ("PROHIBITED", subscription_status_map[SubscriptionStatus::PROHIBITED]);
    EXPECT_EQ("REJECTED", subscription_status_map[SubscriptionStatus::REJECTED]);
    EXPECT_EQ("STALE", subscription_status_map[SubscriptionStatus::STALE]);
    EXPECT_EQ("TIMEOUT", subscription_status_map[SubscriptionStatus::TIMEOUT]);
}

TEST(SubscriptonStatusTest, test_ostream)
{
    std::stringstream ss;
    ss << SubscriptionStatus::UNAVAILABLE;
    EXPECT_EQ("UNAVAILABLE", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::CLOSED;
    EXPECT_EQ("CLOSED", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::CANCELLED;
    EXPECT_EQ("CANCELLED", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::DISCONTINUED;
    EXPECT_EQ("DISCONTINUED", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::EXHAUSTED;
    EXPECT_EQ("EXHAUSTED", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::INACTIVE;
    EXPECT_EQ("INACTIVE", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::OK;
    EXPECT_EQ("OK", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::PENDING;
    EXPECT_EQ("PENDING", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::PROHIBITED;
    EXPECT_EQ("PROHIBITED", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::REJECTED;
    EXPECT_EQ("REJECTED", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::STALE;
    EXPECT_EQ("STALE", ss.str());

    ss.str(std::string());
    ss << SubscriptionStatus::TIMEOUT;
    EXPECT_EQ("TIMEOUT", ss.str());
}

}