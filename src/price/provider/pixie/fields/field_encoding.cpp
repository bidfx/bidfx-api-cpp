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

#include <src/tools/varint.h>
#include <stdexcept>
#include "field_encoding.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::Varint;

void FieldEncodingMethods::SkipFieldValue(FieldEncodingEnum field_encoding, ByteBuffer& buffer)
{
    switch (field_encoding)
    {
        case NOOP:
            break;
        case FIXED1:
            buffer.Seek(1, 1);
            break;
        case FIXED2:
            buffer.Seek(2, 1);
            break;
        case FIXED3:
            buffer.Seek(3, 1);
            break;
        case FIXED4:
            buffer.Seek(4, 1);
            break;
        case FIXED8:
            buffer.Seek(8, 1);
            break;
        case FIXED16:
            buffer.Seek(16, 1);
            break;
        case BYTE_ARRAY:
        {
            uint32_t byte_array_size = Varint::ReadU32(buffer);
            buffer.Seek(byte_array_size, 1);
            break;
        }
        case VARINT:
        {
            uint32_t varint_string_size = Varint::ReadU32(buffer);
            buffer.Seek(varint_string_size - 1, 1);
            break;
        }
        case VARINT_STRING:
        {
            while (!Varint::IsFinalByte(buffer.GetNextByte()))
            {
            }
            break;
        }
        case ZIGZAG:
        {
            while (!Varint::IsFinalByte(buffer.GetNextByte()))
            {
            }
            break;
        }
        default:
        {
            throw std::invalid_argument("unrecognised field encoding code: " + std::to_string(field_encoding) + " ('" +
                                        (char) field_encoding + "')."
            );
        }
    }
}

std::string FieldEncodingMethods::ToString(FieldEncodingEnum field_encoding)
{
    switch (field_encoding)
    {
        case FieldEncodingEnum::NOOP:
            return "NOOP";
        case FieldEncodingEnum::FIXED1:
            return "FIXED1";
        case FieldEncodingEnum::FIXED2:
            return "FIXED2";
        case FieldEncodingEnum::FIXED3:
            return "FIXED3";
        case FieldEncodingEnum::FIXED4:
            return "FIXED4";
        case FieldEncodingEnum::FIXED8:
            return "FIXED8";
        case FieldEncodingEnum::FIXED16:
            return "FIXED16";
        case FieldEncodingEnum::BYTE_ARRAY:
            return "BYTE_ARRAY";
        case FieldEncodingEnum::VARINT:
            return "VARINT";
        case FieldEncodingEnum::VARINT_STRING:
            return "VARINT_STRING";
        case FieldEncodingEnum::ZIGZAG:
            return "ZIGZAG";
        case FieldEncodingEnum::NOT_DEFINED:
            return "NOT_DEFINED";
        default:
            throw std::invalid_argument("unrecognised field encoding code: " +
                                        std::to_string(field_encoding) + " ('" + (char) field_encoding + "').");
    }
}

FieldEncodingEnum FieldEncodingMethods::ValueOf(int code)
{
    switch (code)
    {
        case (char) FieldEncodingEnum::NOOP:
            return FieldEncodingEnum::NOOP;
        case (char) FieldEncodingEnum::FIXED1:
            return FieldEncodingEnum::FIXED1;
        case (char) FieldEncodingEnum::FIXED2:
            return FieldEncodingEnum::FIXED2;
        case (char) FieldEncodingEnum::FIXED3:
            return FieldEncodingEnum::FIXED3;
        case (char) FieldEncodingEnum::FIXED4:
            return FieldEncodingEnum::FIXED4;
        case (char) FieldEncodingEnum::FIXED8:
            return FieldEncodingEnum::FIXED8;
        case (char) FieldEncodingEnum::FIXED16:
            return FieldEncodingEnum::FIXED16;
        case (char) FieldEncodingEnum::BYTE_ARRAY:
            return FieldEncodingEnum::BYTE_ARRAY;
        case (char) FieldEncodingEnum::VARINT:
            return FieldEncodingEnum::VARINT;
        case (char) FieldEncodingEnum::VARINT_STRING:
            return FieldEncodingEnum::VARINT_STRING;
        case (char) FieldEncodingEnum::ZIGZAG:
            return FieldEncodingEnum::ZIGZAG;
        case (char) FieldEncodingEnum::NOT_DEFINED:
            return FieldEncodingEnum::NOT_DEFINED;
        default:
            throw std::invalid_argument("unrecognised field encoding code: " +
                                        std::to_string(code) + " ('" + (char) code + "').");
    }
}

} // namespace bidfx_public_api::price::pixie