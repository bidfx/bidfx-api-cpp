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

#include "src/price/provider/pixie/messages/ack_message.h"
#include "test/tools/varint_test.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::pixie::AckMessage;
using bidfx_public_api::tools::ByteBuffer;
using bidfx_public_api::tools::HexEncoderDecoder;

const std::string kEncodedMessageV1 = "4187ad4be89997de9729b09b97de9729b19b97de9729";
const std::string kEncodedMessageV2 = kEncodedMessageV1 + "9a05";

AckMessage ackMessage = AckMessage(1234567L, 1415120801000L, 1415120801200L, 1415120801201L, 666L);

TEST(AckMessageTest, TestCannotConstructWithNegativeHandlingDuration)
{
    ASSERT_THROW(AckMessage(0, 0, 0, 0, -1234), std::invalid_argument);
}

TEST(AckMessageTest, TestEncode)
{
    ByteBuffer buffer = ackMessage.Encode(3);
    ASSERT_EQ(kEncodedMessageV2, HexEncoderDecoder::StreamAsHex(buffer, 24));
}

TEST(AckMessageTest, TestEncodeV1)
{
    ByteBuffer buffer = ackMessage.Encode(1);
    ASSERT_EQ(kEncodedMessageV1, HexEncoderDecoder::StreamAsHex(buffer, 22));
}

TEST(AckMessageTest, ToString)
{
    ASSERT_EQ(
            "Ack(revision=1234567, revisionTime=20141104170641000, priceReceivedTime=20141104170641200, ackTime=20141104170641201, handlingDuration=666us)",
            ackMessage.ToString());
}

TEST(AckMessageTest, TestEqualsSelf)
{
    EXPECT_EQ(ackMessage, ackMessage);
}

TEST(AckMessageTest, TestEqualsSimilar)
{
    AckMessage newMessage = AckMessage(1234567, 1415120801000L, 1415120801200L, 1415120801201L, 666L);
    EXPECT_EQ(ackMessage, newMessage);
    EXPECT_EQ(std::hash<AckMessage>()(ackMessage), std::hash<AckMessage>()(newMessage));
}

}