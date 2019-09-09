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

#include "src/price/provider/pixie/messages/grant_message.h"
#include "src/price/provider/pixie/pixie_version.h"
#include "src/price/provider/pixie/messages/pixie_message_type.h"
#include "src/tools/buffer_util.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::BufferUtil;

static const std::string ENCODED_GRANT = "477401";
static const std::string ENCODED_DENY = "476614696e76616c69642063726564656e7469616c73";

GrantMessage access_granted = GrantMessage::AccessGranted();
GrantMessage access_denied = GrantMessage::AccessDenied("invalid credentials");

TEST(GrantMessageTest, test_is_granted)
{
    ASSERT_EQ(true, access_granted.IsGranted());
    ASSERT_EQ(false, access_denied.IsGranted());
}

TEST(GrantMessageTest, test_get_reason)
{
    ASSERT_EQ("", access_granted.GetReason());
    ASSERT_EQ("invalid credentials", access_denied.GetReason());
}

TEST(GrantMessageTest, test_encode)
{
    ByteBuffer denied = access_denied.Encode(PixieVersion::CURRENT_VERSION);
    EXPECT_EQ(ENCODED_DENY, BufferUtil::BufferAsHex(denied));
    ByteBuffer grant = access_granted.Encode(PixieVersion::CURRENT_VERSION);
    EXPECT_EQ(ENCODED_GRANT, BufferUtil::BufferAsHex(grant));
}

TEST(GrantMessageTest, test_decode_constructor_with_granted_access_message)
{
    ByteBuffer buffer = BufferUtil::HexBuffer(ENCODED_GRANT);
    ASSERT_EQ(PixieMessageType::GRANT, buffer.ReadByte());

    GrantMessage message = GrantMessage(buffer);
    ASSERT_EQ(access_granted.IsGranted(), message.IsGranted());
    ASSERT_STREQ(access_granted.GetReason().c_str(), message.GetReason().c_str());
}

TEST(GrantMessageTest, test_decode_constructor_with_denied_access_message)
{
    ByteBuffer buffer = BufferUtil::HexBuffer(ENCODED_DENY);
    ASSERT_EQ(PixieMessageType::GRANT, buffer.ReadByte());

    GrantMessage message(buffer);
    ASSERT_EQ(access_denied.IsGranted(), message.IsGranted());
    ASSERT_EQ(access_denied.GetReason(), message.GetReason());
}

TEST(GrantMessageTest, test_encode_and_decode_access_granted_message)
{
    ByteBuffer buffer = access_granted.Encode(PixieVersion::CURRENT_VERSION);
    ASSERT_EQ(PixieMessageType::GRANT, buffer.ReadByte());

    GrantMessage message(buffer);
    ASSERT_EQ(access_granted.IsGranted(), message.IsGranted());
    ASSERT_EQ(access_granted.GetReason(), message.GetReason());
}

TEST(GrantMessageTest, test_encode_and_decode_access_denies_message)
{
    ByteBuffer buffer = access_denied.Encode(PixieVersion::CURRENT_VERSION);
    ASSERT_EQ(PixieMessageType::GRANT, buffer.ReadByte());

    GrantMessage message(buffer);
    ASSERT_EQ(access_denied.IsGranted(), message.IsGranted());
    ASSERT_EQ(access_denied.GetReason(), message.GetReason());
}

TEST(GrantMessageTest, test_to_string)
{
    ASSERT_EQ("Grant(granted=true, reason=)", access_granted.ToString());
}

}