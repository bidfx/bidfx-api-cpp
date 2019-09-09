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

#include "gtest/gtest.h"
#include "include/price/provider/provider.h"

namespace bidfx_public_api::provider
{

using bidfx_public_api::price::provider::provider_status_map;
using bidfx_public_api::price::provider::Provider;

TEST(ProviderStatusTest, test_to_string)
{
    EXPECT_EQ("READY", provider_status_map[Provider::Status::READY]);
    EXPECT_EQ("DOWN", provider_status_map[Provider::Status::DOWN]);
    EXPECT_EQ("CLOSED", provider_status_map[Provider::Status::CLOSED]);
    EXPECT_EQ("UNAVAILABLE", provider_status_map[Provider::Status::UNAVAILABLE]);
    EXPECT_EQ("DISABLED", provider_status_map[Provider::Status::DISABLED]);
    EXPECT_EQ("INVALID", provider_status_map[Provider::Status::INVALID]);
}

TEST(ProviderStatusTest, test_ostream)
{
    std::stringstream ss;
    ss << Provider::Status::READY;
    EXPECT_EQ("READY", ss.str());

    ss.str(std::string());
    ss << Provider::Status::DOWN;
    EXPECT_EQ("DOWN", ss.str());

    ss.str(std::string());
    ss << Provider::Status::CLOSED;
    EXPECT_EQ("CLOSED", ss.str());

    ss.str(std::string());
    ss << Provider::Status::UNAVAILABLE;
    EXPECT_EQ("UNAVAILABLE", ss.str());

    ss.str(std::string());
    ss << Provider::Status::DISABLED;
    EXPECT_EQ("DISABLED", ss.str());

    ss.str(std::string());
    ss << Provider::Status::INVALID;
    EXPECT_EQ("INVALID", ss.str());
}

}