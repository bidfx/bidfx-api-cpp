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

#ifndef PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_FIELD_TYPE_H_
#define PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_FIELD_TYPE_H_

#include <cstdint>
#include <string>
#include "field_encoding.h"

namespace bidfx_public_api::price::pixie
{

enum FieldTypeEnum : unsigned char
{
    UNRECOGNISED = ' ',
    DISCARD = ' ',
    DOUBLE = 'D',
    INTEGER = 'I',
    LONG = 'L',
    STRING = 'S'
};

union FieldTypeUnion
{
    int i;
    double d;
    long l;
    std::string s;
};

class FieldTypeMethods
{
public:
    static std::string ToString(FieldTypeEnum field);
    static FieldTypeEnum ValueOf(int code);
    static FieldEncodingEnum DefaultEncoding(FieldTypeEnum field);
};

} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_FIELD_TYPE_H_