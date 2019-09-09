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

#include <chrono>
#include "src/price/provider/pixie/ack_data.h"
#include "src/price/provider/pixie/messages/ack_message.h"
#include "gtest/gtest.h"

namespace
{

using bidfx_public_api::price::pixie::AckData;
using bidfx_public_api::price::pixie::AckMessage;

const long kRevision = 1234567L;
const long kRevisionTime = 1422808975000L;
const long kPriceReceivedTime = kRevision + 61;
const long kStartNanoTime = 12300000000L;
const long kAckTime = kPriceReceivedTime + 1;

TEST(AckDataTest, ToAckMessageComputesTheHandlingDurationInMicroseconds)
{
    long end_nano_time = (kStartNanoTime + 321000);

    AckData ack_data = AckData(kRevision, kRevisionTime, kPriceReceivedTime, kStartNanoTime);
    EXPECT_EQ(AckMessage(kRevision, kRevisionTime, kPriceReceivedTime, kAckTime, 321),
                        ack_data.ToAckMessage(kAckTime, end_nano_time));
}

TEST(AckDataTest,
     ToAckMessageComputesTheHandlingDurationInMicrosecondsAlwaysAsAPositiveValue)
{
    long end_nano_time = kStartNanoTime - 99000;
    AckData ack_data = AckData
            (kRevision, kRevisionTime, kPriceReceivedTime, kStartNanoTime);
    EXPECT_EQ(AckMessage(kRevision, kRevisionTime, kPriceReceivedTime, kAckTime, 0), ack_data.ToAckMessage(kAckTime, end_nano_time));
}

TEST(AckDataTest,
     ToAckMessageComputesTheHandlingDurationInMicrosecondsByRoundingToNearest)
{
    long end_nano_time = kStartNanoTime + 321000;
    AckData ack_data = AckData(kRevision, kRevisionTime, kPriceReceivedTime, kStartNanoTime);
    EXPECT_EQ(AckMessage(kRevision, kRevisionTime, kPriceReceivedTime, kAckTime, 321), ack_data.ToAckMessage(kAckTime, end_nano_time + 499));
    EXPECT_EQ(AckMessage(kRevision, kRevisionTime, kPriceReceivedTime, kAckTime, 322), ack_data.ToAckMessage(kAckTime, end_nano_time + 999));
    EXPECT_EQ(AckMessage(kRevision, kRevisionTime, kPriceReceivedTime, kAckTime, 322), ack_data.ToAckMessage(kAckTime, end_nano_time + 500));
}

}