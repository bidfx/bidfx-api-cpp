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

#include <string>
#include <unordered_map>
#include "field_def.h"

namespace std
{

using bidfx_public_api::price::pixie::FieldDef;

template<> struct hash<FieldDef>
{
    hash() {}

    typedef FieldDef argument_type;
    typedef int result_type;
    result_type operator()(argument_type const& s) const noexcept
    {
        int hash_code = s.fid_;
        hash_code = static_cast<int>((hash_code * 31u) ^ (!s.name_.empty() ? std::hash<std::string>{}(s.name_) : 0));
        hash_code = (hash_code * 31u) ^ std::hash<std::string>{}(std::to_string(s.encoding_));
        hash_code = (hash_code * 31u) ^ s.scale_;
        return hash_code;
    }
};

}

namespace bidfx_public_api::price::pixie
{

FieldDef::FieldDef(int fid, FieldTypeEnum type, std::string name)
{
    fid_ = fid;
    type_ = type;
    name_ = std::move(name);
    encoding_ = FieldTypeMethods::DefaultEncoding(type);
}

FieldDef::FieldDef(int fid, FieldTypeEnum type, std::string name, FieldEncodingEnum encoding, int scale)
{
    fid_ = fid;
    type_ = type;
    name_ = name;
    encoding_ = encoding;
    scale_ = scale;
}

FieldDef::FieldDef(const bidfx_public_api::price::pixie::FieldDef& to_copy)
{
    fid_ = to_copy.fid_;
    type_ = to_copy.type_;
    name_ = to_copy.name_;
    encoding_ = to_copy.encoding_;
    scale_ = to_copy.scale_;
}

FieldDef::FieldDef(bidfx_public_api::price::pixie::FieldDef&& to_move) : FieldDef(to_move)
{}

FieldDef& FieldDef::operator=(const bidfx_public_api::price::pixie::FieldDef& to_copy)
{
    fid_ = to_copy.fid_;
    type_ = to_copy.type_;
    name_ = to_copy.name_;
    encoding_ = to_copy.encoding_;
    scale_ = to_copy.scale_;
    return *this;
}

FieldDef& FieldDef::operator=(FieldDef&& to_move)
{
    return operator=(to_move);
}

bool FieldDef::operator==(const FieldDef& that) const
{
    return fid_ == that.fid_ && name_ == that.name_ &&
    encoding_ == that.encoding_ && scale_ == that.scale_;
}

bool FieldDef::operator!=(const FieldDef& that) const
{
    return !(*this == that);
}

size_t FieldDef::GetByteSize() const
{
    return sizeof(fid_) + sizeof(type_) + sizeof(char) * name_.length() + sizeof(encoding_) + sizeof(scale_);
}

const std::string& FieldDef::GetName() const
{
    return name_;
}

std::string FieldDef::ToString() const
{
    return std::string("fieldDef(FID=" + std::to_string(fid_) +
                       ", name=\"" + name_ + "\", type=" + FieldTypeMethods::ToString(type_) + ", encoding="
                       + FieldEncodingMethods::ToString(encoding_) + ", scale=" + std::to_string(scale_) + ')'
    );
}

} // namespace bidfx_public_api::price::pixie