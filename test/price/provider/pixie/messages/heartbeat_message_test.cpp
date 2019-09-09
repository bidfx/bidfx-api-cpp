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
#include "src/tools/buffer_util.h"
#include "src/price/provider/pixie/messages/pixie_message_type.h"
#include "src/price/provider/pixie/pixie_version.h"
#include "src/price/provider/pixie/messages/heartbeat_message.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::BufferUtil;

TEST(HeartbeatMessageTest, test_encode)
{
    ByteBuffer buffer = HeartbeatMessage().Encode(PixieVersion::CURRENT_VERSION);
    EXPECT_EQ("48", BufferUtil::BufferAsHex(buffer));
}

TEST(HeartbeatMessageTest, test_decode_constructor)
{
    ByteBuffer buffer = BufferUtil::HexBuffer("48");
    EXPECT_EQ(PixieMessageType::HEARTBEAT, buffer.ReadByte());
}

TEST(HeartbeatMessageTest, test_to_string)
{
    HeartbeatMessage message;
    EXPECT_EQ("Heartbeat()", message.ToString());
}

TEST(HeartbeatMessageTest, test_equals_self)
{
    HeartbeatMessage message;
    EXPECT_EQ(message, message);
}

TEST(HeartbeatMessageTest, test_equals_similar)
{
    HeartbeatMessage message1;
    HeartbeatMessage message2;
    EXPECT_EQ(message1, message2);
}

TEST(HeartbeatMessageTest, test_not_equals_different_type_of_message)
{
    HeartbeatMessage message1;
    AckMessage message2 = AckMessage(0, 0, 0, 0, 0);
    EXPECT_NE(message1, message2);
}



}