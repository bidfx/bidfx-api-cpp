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

#include "field_type.h"

namespace bidfx_public_api::price::pixie
{

std::string FieldTypeMethods::ToString(FieldTypeEnum field_type)
{
    switch (field_type)
    {
        case FieldTypeEnum::DOUBLE:
            return "DOUBLE";
        case FieldTypeEnum::INTEGER:
            return "INTEGER";
        case FieldTypeEnum::LONG:
            return "LONG";
        case FieldTypeEnum::STRING:
            return "STRING";
        case FieldTypeEnum::DISCARD:
            return "DISCARD";
        default:
            return "DOUBLE";
    }
}

FieldTypeEnum FieldTypeMethods::ValueOf(int code)
{
    switch (code)
    {
        case (char) FieldTypeEnum::DOUBLE:
            return FieldTypeEnum::DOUBLE;
        case (char) FieldTypeEnum::INTEGER:
            return FieldTypeEnum::INTEGER;
        case (char) FieldTypeEnum::LONG:
            return FieldTypeEnum::LONG;
        case (char) FieldTypeEnum::STRING:
            return FieldTypeEnum::STRING;
        default:
            return FieldTypeEnum::UNRECOGNISED;
    }
}

FieldEncodingEnum FieldTypeMethods::DefaultEncoding(FieldTypeEnum field)
{
    switch(field)
    {
        case UNRECOGNISED:
            return FieldEncodingEnum::NOOP;
        case DOUBLE:
            return FieldEncodingEnum::FIXED8;
        case INTEGER:
            return FieldEncodingEnum::FIXED4;
        case LONG:
            return FieldEncodingEnum::FIXED8;
        case STRING:
            return FieldEncodingEnum::VARINT_STRING;
    }
}

} // namespace bidfx_public_api::price::pixie