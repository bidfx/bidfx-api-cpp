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

#include "buffer_util.h"
#include "hex_encoder.h"

namespace bidfx_public_api::tools
{

ByteBuffer BufferUtil::HexBuffer(const std::string& hex_string)
{
    std::vector<unsigned char> bytes = HexEncoder::DecodeFromHex(hex_string);
    ByteBuffer buffer(bytes.size());
    buffer.WriteBytes(bytes.data(), bytes.size());
    return std::move(buffer);
}

std::string BufferUtil::BufferAsHex(const ByteBuffer& buffer)
{
    return HexEncoder::EncodeAsHex(buffer.ToArray(), buffer.ReadableBytes());
}

std::string BufferUtil::BufferAsString(const ByteBuffer& buffer)
{
    return std::string(reinterpret_cast<char*>(buffer.ToArray()), buffer.ReadableBytes());
}

ByteBuffer BufferUtil::StringAsBuffer(const std::string& s)
{
    ByteBuffer buffer(s.length());
    buffer.WriteString(s);
    return std::move(buffer);
}

}