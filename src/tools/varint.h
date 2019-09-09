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

#ifndef PUBLIC_API_CPP_TOOLS_VARINT_H_
#define PUBLIC_API_CPP_TOOLS_VARINT_H_

#include <string>
#include <vector>
#include <memory>
#include "byte_buffer.h"

namespace bidfx_public_api::tools
{

/**
 * This class provides variable-length integer-type encoding and decoding.
 * @author Frazer Bennett Wilford
 */
class Varint
{
private:
    static const uint8_t kBitsPerByte = 7u;
    static const uint8_t kContinuationBit = 1u << kBitsPerByte;
    static const uint8_t kMaskLow7Bits = kContinuationBit - 1u;
public:
    static const std::string kNullString;

    static uint32_t ReadU32(InputStream& stream);

    static uint64_t ReadU64(InputStream& stream);

    static void WriteU32(OutputStream& stream, uint32_t value);

    static void WriteU64(OutputStream& stream, uint64_t value);

    static bool IsFinalByte(int b);

    static std::string ReadString(InputStream& stream);

    static void WriteString(OutputStream& stream, std::string s);

    static std::vector<std::string> ReadStringVector(InputStream& stream);

    static void WriteStringVector(OutputStream& stream, const std::vector<std::string>& arr);

    static long ZigzagToLong(unsigned long long zigzag);

    static int ZigzagToInt(unsigned int zigzag);
};

} // namespace bidfx_public_api::tools

#endif //PUBLIC_API_CPP_TOOLS_VARINT_H_
