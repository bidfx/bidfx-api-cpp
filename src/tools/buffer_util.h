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

#ifndef PUBLIC_API_CPP_TOOLS_BUFFER_UTIL_H_
#define PUBLIC_API_CPP_TOOLS_BUFFER_UTIL_H_

#include "byte_buffer.h"
#include "memory"

namespace bidfx_public_api::tools
{

/**
 * This class provides utilities for managing buffers.
 * @author Frazer Bennett Wilford
 */
class BufferUtil
{
public:
    /**
     * Converts a string of hexadecimal numbers representing byte value into a byte buffer. The input string may optionally
     * contain spaces between the hex digits.
     * @param hexString the input hex string.
     * @return a byte buffer.
     */
    static ByteBuffer HexBuffer(const std::string& hex_string);

    /**
     * Converts a byte buffer into a string of hex digits representing the bytes in the buffer.
     * @param buffer the buffer to convert.
     * @return the hex string.
     */
    static std::string BufferAsHex(const ByteBuffer& buffer);

    /**
     * Converts a byte buffer into a string representing the UTF8 encoding of bytes in the buffer.
     * @param buffer the buffer to convert.
     * @return the converted string.
     */
    static std::string BufferAsString(const ByteBuffer& buffer);

    /**
    * Converts a string into a channel buffer by encoding it as UTF8.
    * @param s the string to convert.
    * @return the encoded byte buffer.
    */
    static ByteBuffer StringAsBuffer(const std::string& s);
};


}

#endif //PUBLIC_API_CPP_TOOLS_BUFFER_UTIL_H_
