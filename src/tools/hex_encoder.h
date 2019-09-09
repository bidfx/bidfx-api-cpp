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

#ifndef PUBLIC_API_CPP_TOOLS_HEX_ENCODER_H_
#define PUBLIC_API_CPP_TOOLS_HEX_ENCODER_H_

#include <vector>
#include <string>
#include <array>

namespace bidfx_public_api::tools
{

/**
 * This class provides encoding into hexadecimal for bytes arrays.
 * @author Frazer Bennett Wilford
 */
class HexEncoder
{
private:
    static const std::vector<char> HEX_CHARS;

    static unsigned char DecodeByte(unsigned char c1, unsigned char c2);

    static unsigned int DecodeNibble(unsigned char c);

public:

    /**
     * Encodes an array of bytes as hexadecimal.
     * @param bytes the byte array to encode.
     * @return a string of hexadecimal digits.
     */
    static std::string EncodeAsHex(unsigned char* bytes, size_t len);

    /**
     * Encodes an array of bytes as hexadecimal.
     * @param bytes the byte array to encode.
     * @param offset the offset of data to encode.
     * @param length the length of data to encode.
     * @return a string of hexadecimal digits.
     */
    static std::string EncodeAsHex(unsigned char* bytes, int offset, size_t len_to_read, size_t len_of_bytes);

    /**
     * Decodes a string of hexadecimal digits into an array of bytes.
     * @param hex the hex string to decode.
     * @return the decoded array.
     * @throws NumberFormatException if the given string is not a valid hex string.
     */
    static std::vector<unsigned char> DecodeFromHex(std::string);

};

}


#endif //PUBLIC_API_CPP_TOOLS_HEX_ENCODER_H_
