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

#ifndef PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_FIELD_ENCODING_H_
#define PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_FIELD_ENCODING_H_

#include <src/tools/byte_buffer.h>

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::ByteBuffer;

enum FieldEncodingEnum : uint8_t
{
    NOOP = '0',
    FIXED1 = '1',
    FIXED2 = '2',
    FIXED3 = '3',
    FIXED4 = '4',
    FIXED8 = '8',
    FIXED16 = '@',
    BYTE_ARRAY = 'B',
    VARINT_STRING = 'S',
    VARINT = 'V',
    ZIGZAG = 'Z',
    NOT_DEFINED = 0
};

class FieldEncodingMethods
{
public:
    static void SkipFieldValue(FieldEncodingEnum field_encoding, ByteBuffer& buffer);
    static std::string ToString(FieldEncodingEnum field_encoding);
    static FieldEncodingEnum ValueOf(int code);
};

} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_FIELD_ENCODING_H_
