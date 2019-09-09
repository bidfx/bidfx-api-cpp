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

#ifndef PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_FIELD_DEF_H_
#define PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_FIELD_DEF_H_

#include <string>
#include "field_type.h"
#include "field_encoding.h"

namespace bidfx_public_api::price::pixie
{

class FieldDef
{

public:
    static const int REFERENCING_FID = INT32_MAX;
    int fid_;
    std::string name_;
    FieldTypeEnum type_ = FieldTypeEnum::UNRECOGNISED;
    FieldEncodingEnum encoding_ = FieldEncodingEnum::NOT_DEFINED;
    int scale_;

    FieldDef(int fid, FieldTypeEnum type, std::string name);

    FieldDef(int fid, FieldTypeEnum type, std::string name, FieldEncodingEnum encoding, int scale);

    FieldDef(const FieldDef& to_copy);

    FieldDef(FieldDef&& to_move);

    FieldDef& operator=(const FieldDef& to_copy);

    FieldDef&operator=(FieldDef &&to_move);

    bool operator==(const FieldDef& that) const;

    bool operator!=(const FieldDef& that) const;

    size_t GetByteSize() const;

    const std::string& GetName() const;

    std::string ToString() const;
};

inline std::ostream & operator<<(std::ostream & str, FieldDef & v)
{
    return str << v.ToString();
}

} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_FIELD_DEF_H_
