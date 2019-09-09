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

#include <sstream>
#include "src/tools/varint.h"
#include "data_dictionary_message.h"
#include "pixie_message_type.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::Varint;

DataDictionaryMessage::DataDictionaryMessage(std::vector<FieldDef> field_defs, bool update) : field_defs_ {FilterUnwantedFields(field_defs)}
{
    options_ = update ? 2 : 0;
}

DataDictionaryMessage::DataDictionaryMessage(ByteBuffer& buffer)
{
    options_ = Varint::ReadU32(buffer);
    size_t size = Varint::ReadU32(buffer);
    for (int i = 0; i < size; i++)
    {
        field_defs_.push_back(ReadFieldDef(buffer));
    }
}

bool DataDictionaryMessage::IsUpdate() const
{
    return ((options_ & (1u << 1u)) != 0);
}

std::vector<FieldDef> DataDictionaryMessage::FilterUnwantedFields(std::vector<FieldDef> fields) const
{
    std::vector<FieldDef> filtered_fields;
    for (FieldDef field : fields)
    {
        if (field.type_ != FieldTypeEnum::DISCARD && field.type_ != FieldTypeEnum::UNRECOGNISED)
        {
            filtered_fields.push_back(field);
        }
    }
    return filtered_fields;
}

FieldDef DataDictionaryMessage::ReadFieldDef(ByteBuffer& buffer)
{
    int fid = Varint::ReadU32(buffer);
    FieldTypeEnum field_type = FieldTypeMethods::ValueOf(buffer.ReadByte());
    FieldEncodingEnum encoding = FieldEncodingMethods::ValueOf(buffer.ReadByte());
    int scale =  Varint::ReadU32(buffer);
    std::string name = Varint::ReadString(buffer);

    return FieldDef(fid, field_type, name, encoding, scale);
}

const std::vector<FieldDef> DataDictionaryMessage::GetFieldDefs() const
{
    return field_defs_;
}

const std::string DataDictionaryMessage::ToString() const
{
    std::stringstream ss;
    std::string update = IsUpdate() ? "true" : "false";
    ss << "DataDictionary(update=" << update << ", fields=[\n  ";

    FieldDef field = field_defs_.at(0);
    ss <<  field << "\n";

    for (size_t i = 1; i < field_defs_.size(); i++)
    {
        field = field_defs_.at(i);
        ss << "  " << field << "\n";
    }

    ss << "])";
    return ss.str();
}

bool DataDictionaryMessage::operator==(const DataDictionaryMessage& that) const
{
    if (IsUpdate()!= that.IsUpdate())
    {
        return false;
    }

    for (size_t i = 0; i < field_defs_.size(); i++)
    {
        if (field_defs_.at(i) != that.GetFieldDefs().at(i))
        {
            return false;
        }
    }

    return true;
}

bool DataDictionaryMessage::operator!=(const DataDictionaryMessage& that) const
{
    return !(*this == that);
}

}
