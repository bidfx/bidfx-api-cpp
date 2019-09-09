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
#include "hex_encoder.h"

namespace bidfx_public_api::tools
{

const std::vector<char> HexEncoder::HEX_CHARS = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

std::string HexEncoder::EncodeAsHex(unsigned char* bytes, size_t len)
{
    std::stringstream ss;

    for (unsigned long i = 0u; i < len; i++)
    {
        ss << HexEncoder::HEX_CHARS[((unsigned int) bytes[i] >> 4u & 0xfu)];
        ss << HexEncoder::HEX_CHARS[(unsigned int) bytes[i] & 0xfu];
    }

    return ss.str();
}

std::string HexEncoder::EncodeAsHex(unsigned char* bytes, int offset, size_t len_to_read, size_t len_of_bytes)
{
    std::stringstream ss;

    for (int index = offset; index < offset + len_to_read && index < len_of_bytes; ++index)
    {
        unsigned int byte = (unsigned int) bytes[index];

        ss << HEX_CHARS[byte >> 4u & 0xfu];
        ss << HEX_CHARS[byte & 0xfu];
    }

    return ss.str();
}

std::vector<unsigned char> HexEncoder::DecodeFromHex(std::string hex)
{
    size_t hex_len = hex.length() + 1u >> 1u;
    std::vector<unsigned char> bytes;

    int byte_index = 0;
    unsigned long char_index = 0u;

    if ((hex.length() & 1u) == 1u)
    {
        bytes.push_back((unsigned char) DecodeNibble(static_cast<unsigned char>(hex.at(char_index++))));
        byte_index++;
    }

    while (byte_index <  hex_len)
    {
        unsigned char c1 = static_cast<unsigned char>(hex.at(char_index++));
        unsigned char c2 = static_cast<unsigned char>(hex.at(char_index++));

        bytes.push_back(DecodeByte(c1, c2));
        byte_index += 1;
    }

    return bytes;
}

unsigned char HexEncoder::DecodeByte(unsigned char c1, unsigned char c2)
{
    unsigned int high = DecodeNibble(c1);
    unsigned int low = DecodeNibble(c2);
    return (unsigned char)((high & 0x0fu) << 4u | low);
}

unsigned int HexEncoder::DecodeNibble(unsigned char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10u;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10u;

    std::stringstream error_message;
    error_message << "cannot decode non-hex character: " << c << std::endl;
    throw std::invalid_argument(error_message.str());
}

}