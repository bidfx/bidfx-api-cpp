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

#include "src/price/provider/pixie/messages/login_message.h"
#include "src/price/provider/pixie/pixie_version.h"
#include "src/tools/buffer_util.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::BufferUtil;

const std::string APPLICATION = "TradeSmart";
const std::string APPLICATION_VERSION = "3.7.0.88";
const std::string USERNAME = "acozzoli";
const std::string PASSWORD = "g1veMes0Mesugar";
const std::string ALIAS = "nello";
const char MSG_TYPE = 'L';
const std::string API = "Bidfx - C++";
const std::string API_VERSION = "1.0";
const std::string PRODUCT_SERIAL_NUMBER = "29383BID";
const std::string ENCODED_MESSAGE_V1 = "L\011acozzoli\020g1veMes0Mesugar\006nello";
const std::string ENCODED_MESSAGE_V2 = ENCODED_MESSAGE_V1 + "\013TradeSmart\0113.7.0.88";
const std::string ENCODED_MESSAGE_V4 = ENCODED_MESSAGE_V2 + "\014Bidfx - C++\0041.0\011BidFXCpp\01129383BID";
LoginMessage login_message = LoginMessage(USERNAME, PASSWORD, ALIAS, APPLICATION, APPLICATION_VERSION, API, API_VERSION, PRODUCT_SERIAL_NUMBER);

LoginMessage Decode(ByteBuffer& buffer, int version)
{
    buffer.ReadByte();
    LoginMessage message(buffer, version);
    return message;
}

LoginMessage EncodeAndDecode(LoginMessage& message)
{
    ByteBuffer encoded_message = message.Encode(PixieVersion::CURRENT_VERSION);
    LoginMessage decoded_message = Decode(encoded_message, PixieVersion::CURRENT_VERSION);
    return decoded_message;
}

TEST(LoginMessageTest, returns_given_values)
{
    EXPECT_EQ(USERNAME, login_message.GetUsername());
    EXPECT_EQ(PASSWORD, login_message.GetPassword());
    EXPECT_EQ(APPLICATION, login_message.GetApplication());
    EXPECT_EQ(APPLICATION_VERSION, login_message.GetApplicationVersion());
    EXPECT_EQ(ALIAS, login_message.GetAlias());
    EXPECT_EQ(API, login_message.GetApi());
    EXPECT_EQ(API_VERSION, login_message.GetApiVersion());
    EXPECT_EQ("BidFXCpp", login_message.GetProduct());
    EXPECT_EQ(PRODUCT_SERIAL_NUMBER, login_message.GetProductSerialNumber());
}

TEST(LoginMessageTest, test_encode_version1)
{
    ByteBuffer buffer = login_message.Encode(PixieVersion::VERSION1);
    EXPECT_EQ(ENCODED_MESSAGE_V1, BufferUtil::BufferAsString(buffer));
}

TEST(LoginMessageTest, test_encode_version2)
{
    ByteBuffer buffer = login_message.Encode(PixieVersion::VERSION2);
    EXPECT_EQ(ENCODED_MESSAGE_V2, BufferUtil::BufferAsString(buffer));
}

TEST(LoginMessageTest, test_encode_version4)
{
    ByteBuffer buffer = login_message.Encode(PixieVersion::VERSION4);
    EXPECT_EQ(ENCODED_MESSAGE_V4, BufferUtil::BufferAsString(buffer));
}

TEST(LoginMessageTest, test_encode_current_version)
{
    ByteBuffer buffer = login_message.Encode(PixieVersion::CURRENT_VERSION);
    EXPECT_EQ(ENCODED_MESSAGE_V4, BufferUtil::BufferAsString(buffer));
}

TEST(LoginMessageTest, test_decode_version1)
{
    ByteBuffer buffer = BufferUtil::StringAsBuffer(ENCODED_MESSAGE_V1);
    LoginMessage decoded = Decode(buffer, PixieVersion::VERSION1);
    EXPECT_EQ(USERNAME, decoded.GetUsername());
    EXPECT_EQ(PASSWORD, decoded.GetPassword());
    EXPECT_EQ(ALIAS, decoded.GetAlias());
}

TEST(LoginMessageTest, test_decode_version2)
{
    ByteBuffer buffer = BufferUtil::StringAsBuffer(ENCODED_MESSAGE_V2);
    LoginMessage decoded = Decode(buffer, PixieVersion::VERSION2);
    EXPECT_EQ(USERNAME, decoded.GetUsername());
    EXPECT_EQ(PASSWORD, decoded.GetPassword());
    EXPECT_EQ(ALIAS, decoded.GetAlias());
    EXPECT_EQ(APPLICATION, decoded.GetApplication());
    EXPECT_EQ(APPLICATION_VERSION, decoded.GetApplicationVersion());
}

TEST(LoginMessageTest, test_decode_version4)
{
    ByteBuffer buffer = BufferUtil::StringAsBuffer(ENCODED_MESSAGE_V4);
    LoginMessage decoded = Decode(buffer, PixieVersion::VERSION4);
    EXPECT_EQ(USERNAME, decoded.GetUsername());
    EXPECT_EQ(PASSWORD, decoded.GetPassword());
    EXPECT_EQ(ALIAS, decoded.GetAlias());
    EXPECT_EQ(APPLICATION, decoded.GetApplication());
    EXPECT_EQ(APPLICATION_VERSION, decoded.GetApplicationVersion());
    EXPECT_EQ(API, decoded.GetApi());
    EXPECT_EQ(API_VERSION, decoded.GetApiVersion());
    EXPECT_EQ("BidFXCpp", decoded.GetProduct());
    EXPECT_EQ(PRODUCT_SERIAL_NUMBER, decoded.GetProductSerialNumber());
}

TEST(LoginMessageTest, test_encode_and_decode)
{
    EXPECT_EQ(login_message, EncodeAndDecode(login_message));
    LoginMessage message1("mcandate", PASSWORD, ALIAS, APPLICATION, APPLICATION_VERSION, API, API_VERSION, PRODUCT_SERIAL_NUMBER);
    LoginMessage message2(USERNAME, "g1veMes0Mehoney", ALIAS, APPLICATION, APPLICATION_VERSION, API, API_VERSION, PRODUCT_SERIAL_NUMBER);
    LoginMessage message3(USERNAME, PASSWORD, "matteo", APPLICATION, APPLICATION_VERSION, API, API_VERSION, PRODUCT_SERIAL_NUMBER);
    EXPECT_NE(login_message, EncodeAndDecode(message1));
    EXPECT_NE(login_message, EncodeAndDecode(message2));
    EXPECT_NE(login_message, EncodeAndDecode(message3));
}

TEST(LoginMessageTest, test_returns_formatted_string)
{
    EXPECT_EQ("Login(username=acozzoli, password=***, alias=nello, application=TradeSmart, version=3.7.0.88, api=Bidfx - C++, api_version=1.0, product=BidFXCpp, product_serial_number=29383BID)",
                 login_message.ToString());
}

TEST(LoginMessageTest, test_equals)
{
    EXPECT_EQ(login_message, LoginMessage(USERNAME, PASSWORD, ALIAS, APPLICATION, APPLICATION_VERSION, API, API_VERSION, PRODUCT_SERIAL_NUMBER));
    EXPECT_NE(login_message, LoginMessage("mcandate", PASSWORD, ALIAS, APPLICATION, APPLICATION_VERSION, API, API_VERSION, PRODUCT_SERIAL_NUMBER));
    EXPECT_NE(login_message, LoginMessage(USERNAME, "g1veMes0Mehoney", ALIAS, APPLICATION, APPLICATION_VERSION, API, API_VERSION, PRODUCT_SERIAL_NUMBER));
    EXPECT_NE(login_message, LoginMessage(USERNAME, PASSWORD, "matteo", APPLICATION, APPLICATION_VERSION, API, API_VERSION, PRODUCT_SERIAL_NUMBER));
}

}