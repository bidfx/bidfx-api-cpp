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

#include "include/tenor.h"
#include "gtest/gtest.h"

namespace bidfx_public_api
{

TEST(TenorTest, test_valid_near_far_tenor_validation)
{
    EXPECT_TRUE(Tenor::IsValidNearFarTenor(Tenor::TODAY, Tenor::TOMORROW));
    EXPECT_TRUE(Tenor::IsValidNearFarTenor(Tenor::TODAY, Tenor::IN_1_WEEK));
    EXPECT_TRUE(Tenor::IsValidNearFarTenor(Tenor::SPOT, Tenor::IN_1_YEAR));
    EXPECT_TRUE(Tenor::IsValidNearFarTenor(Tenor::IN_11_MONTHS, Tenor::IN_1_YEAR));
}

TEST(TenorTest, test_invalid_near_far_tenor_validation)
{
    EXPECT_FALSE(Tenor::IsValidNearFarTenor(Tenor::TOMORROW, Tenor::TODAY));
    EXPECT_FALSE(Tenor::IsValidNearFarTenor(Tenor::IN_11_MONTHS, Tenor::IN_1_WEEK));
    EXPECT_FALSE(Tenor::IsValidNearFarTenor(Tenor::IN_1_YEAR, Tenor::IN_11_MONTHS));
    EXPECT_FALSE(Tenor::IsValidNearFarTenor(Tenor::SPOT_NEXT, Tenor::SPOT));
    EXPECT_FALSE(Tenor::IsValidNearFarTenor(Tenor::SPOT_NEXT, Tenor::TODAY));
}

}