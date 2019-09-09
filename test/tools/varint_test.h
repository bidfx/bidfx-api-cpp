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

//
// Created by mrobinso on 18/09/2018.
//

#ifndef PUBLIC_API_CPP_TEST_TOOLS_VARINT_TEST_H_
#define PUBLIC_API_CPP_TEST_TOOLS_VARINT_TEST_H_

#include <string>
#include "src/tools/byte_buffer.h"

namespace bidfx_public_api::tools
{

using bidfx_public_api::tools::ByteBuffer;

class HexEncoderDecoder
{
public:
    static ByteBuffer WriteStringToStream(std::string s);

    static std::string WriteU32ToHexString(int value, uint32_t size);

    static std::string WriteU64ToHexString(long value, uint32_t size);

    static ByteBuffer WriteU32ToNewStream(int value);

    static ByteBuffer WriteU64ToNewStream(long value);

    static std::string StreamAsHex(ByteBuffer& buffer, uint32_t size);

    static ByteBuffer HexStream(std::string hex_string);

    static std::string EncodeAsHex(uint8_t* bytes, uint32_t size);

    static uint8_t* DecodeFromHex(std::string hex);

    static int DecodeNibble(char c);

    static uint8_t DecodeByte(char c1, char c2);

};

}

#endif //PUBLIC_API_CPP_TEST_TOOLS_VARINT_TEST_H_
