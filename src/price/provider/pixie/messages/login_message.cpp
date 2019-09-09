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

#include <sstream>
#include "src/price/provider/pixie/pixie_version.h"
#include "src/tools/varint.h"
#include "login_message.h"
#include "pixie_message_type.h"

namespace bidfx_public_api::price::pixie
{

LoginMessage::LoginMessage(std::string username, std::string password, std::string alias, std::string application,
                           std::string application_version, std::string api, std::string api_version, std::string product_serial_number)
{
    username_ = username;
    password_ = password;
    alias_ = alias;
    application_ = application;
    application_version_ = application_version;
    api_ = api;
    api_version_ = api_version;
    product_ = "BidFXCpp";
    product_serial_number_ = product_serial_number;
}

LoginMessage::LoginMessage(ByteBuffer& message_frame, int version)
{
    username_ = Varint::ReadString(message_frame);
    password_ = Varint::ReadString(message_frame);
    alias_ = Varint::ReadString(message_frame);

    bool version2 = version >= PixieVersion::VERSION2;
    application_ = version2 ? Varint::ReadString(message_frame) : "";
    application_version_ = version2 ? Varint::ReadString(message_frame) : "";

    bool version4 = version >= PixieVersion::VERSION4;
    api_ = version4 ? Varint::ReadString(message_frame) : "";
    api_version_ = version4 ? Varint::ReadString(message_frame) : "";
    product_ = version4 ? Varint::ReadString(message_frame) : "";
    product_serial_number_ = version4 ? Varint::ReadString(message_frame) : "";
}

ByteBuffer LoginMessage::Encode(int version) const
{
    size_t message_len = sizeof(PixieMessageType::LOGIN) + sizeof(unsigned char) * username_.size() +
                      sizeof(unsigned char) * password_.size() + sizeof(unsigned char) * alias_.size() +
                      sizeof(unsigned char) * password_.size();

    bool version2 = version >= PixieVersion::VERSION2;
    if (version2)
    {
        message_len += sizeof(unsigned char) * application_.size() + sizeof(unsigned char) * application_version_.size();
    }

    bool version4 = version >= PixieVersion::VERSION4;
    if (version4)
    {
        message_len += sizeof(unsigned char) * api_.size() + sizeof(unsigned char) * api_version_.size() +
                       sizeof(unsigned char) * product_.size() + sizeof(unsigned char) * product_serial_number_.size();
    }

    ByteBuffer message_frame(message_len);
    message_frame.WriteByte(PixieMessageType::LOGIN);
    Varint::WriteString(message_frame, username_);
    Varint::WriteString(message_frame, password_);
    Varint::WriteString(message_frame, alias_);

    if (version2)
    {
        Varint::WriteString(message_frame, application_);
        Varint::WriteString(message_frame, application_version_);
    }

    if (version4)
    {
        Varint::WriteString(message_frame, api_);
        Varint::WriteString(message_frame, api_version_);
        Varint::WriteString(message_frame, product_);
        Varint::WriteString(message_frame, product_serial_number_);
    }

    return std::move(message_frame);
}

std::string LoginMessage::GetUsername()
{
    return username_;
}

std::string LoginMessage::GetPassword()
{
    return password_;
}

std::string LoginMessage::GetAlias()
{
    return alias_;
}

std::string LoginMessage::GetApplication()
{
    return application_;
}

std::string LoginMessage::GetApplicationVersion()
{
    return application_version_;
}

std::string LoginMessage::GetApi()
{
    return api_;
}

std::string LoginMessage::GetApiVersion()
{
    return api_version_;
}

std::string LoginMessage::GetProduct()
{
    return product_;
}

std::string LoginMessage::GetProductSerialNumber()
{
    return product_serial_number_;
}

const bool LoginMessage::operator==(const LoginMessage& other) const
{
    return  username_ == other.username_ &&
            password_ == other.password_ &&
            alias_ == other.alias_ &&
            application_ == other.application_ &&
            application_version_ == other.application_version_ &&
            api_ == other.api_ && api_version_ == other.api_version_ &&
            product_ == other.product_ &&
            product_serial_number_ == other.product_serial_number_;
}

const bool LoginMessage::operator!=(const LoginMessage& other) const
{
    return !(*this == other);
}

const std::string LoginMessage::ToString()
{
    std::stringstream ss;
    ss << "Login(username=" << username_ << ", password=***" << ", alias=" << alias_ <<
       ", application=" << application_ << ", version=" << application_version_ <<
       ", api=" << api_ << ", api_version=" << api_version_ << ", product=" << product_ <<
       ", product_serial_number=" << product_serial_number_ << ")";
    return ss.str();
}

}