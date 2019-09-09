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

#include "src/price/provider/pixie/messages/welcome_message.h"
#include "src/price/provider/pixie/pixie_version.h"
#include "src/tools/buffer_util.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::BufferUtil;

int VERSION = 1;
int CLIENT_ID = 4321;
int SERVER_ID = 9902;
std::string ENCODED_MESSAGE = "570001000010e1000026ae";
char MSG_TYPE = 'W';
WelcomeMessage WELCOME_MESSAGE = WelcomeMessage(VERSION, CLIENT_ID, SERVER_ID);

WelcomeMessage ReadWelcomeMessage(std::string encoded_message)
{
    ByteBuffer buffer = BufferUtil::HexBuffer(encoded_message);
    buffer.ReadByte();
    return WelcomeMessage(buffer);
}

TEST(WelcomeMessageTest, test_get_version)
{
    ASSERT_EQ(VERSION, WELCOME_MESSAGE.GetVersion());
}

TEST(WelcomeMessageTest, test_get_client_ID)
{
    ASSERT_EQ(CLIENT_ID, WELCOME_MESSAGE.GetClientID());
}

TEST(WelcomeMessageTest, test_get_server_ID)
{
    ASSERT_EQ(SERVER_ID, WELCOME_MESSAGE.GetServerID());
}

TEST(WelcomeMessageTest, test_get_options_is_always_zero_after_creation)
{
    ASSERT_EQ(0, WELCOME_MESSAGE.GetOptions());
}

TEST(WelcomeMessageTest, test_get_options_may_be_read_from_the_wire)
{
    WelcomeMessage message = ReadWelcomeMessage("570701000010e1000026ae");
    ASSERT_EQ(7, message.GetOptions());
}

TEST(WelcomeMessageTest, test_encodes_a_message_into_a_buffer)
{
    ByteBuffer buffer = WELCOME_MESSAGE.Encode(PixieVersion::CURRENT_VERSION);
    ASSERT_EQ(ENCODED_MESSAGE, BufferUtil::BufferAsHex(buffer));
}

TEST(WelcomeMessageTest, test_decodes_a_buffer_into_a_message)
{
    WelcomeMessage message = ReadWelcomeMessage(ENCODED_MESSAGE);
    EXPECT_EQ(WELCOME_MESSAGE, message);
}

TEST(WelcomeMessageTest, test_equals_self)
{
    EXPECT_EQ(WELCOME_MESSAGE, WELCOME_MESSAGE);
}

TEST(WelcomeMessageTest, test_equals_different)
{
    EXPECT_NE(WELCOME_MESSAGE, WelcomeMessage(VERSION + 1, CLIENT_ID, SERVER_ID));
    EXPECT_NE(WELCOME_MESSAGE, WelcomeMessage(VERSION, CLIENT_ID + 1, SERVER_ID));
    EXPECT_NE(WELCOME_MESSAGE, WelcomeMessage(VERSION, CLIENT_ID, SERVER_ID + 1));
}

TEST(WelcomeMessageTest, test_to_string)
{
    WelcomeMessage message = WelcomeMessage(1, 3892831073, 3728490324);
    ASSERT_EQ("Welcome(version=1, clientID=0xe807e361, serverID=0xde3c3f54)", message.ToString());
}

}