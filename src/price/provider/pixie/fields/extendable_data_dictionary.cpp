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

#include <stdexcept>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "extendable_data_dictionary.h"
#include "data_dictionary_utils.h"

namespace bidfx_public_api::price::pixie
{

std::optional<FieldDef> ExtendableDataDictionary::FieldDefByFid(int fid)
{
    if (fid < 0)
    {
        return {};
    }
    else
    {
        auto it = field_defs_by_fid_.find(fid);
        if (it == field_defs_by_fid_.end())
        {
            return {};
        }
        return it->second;
    }
}

std::optional<FieldDef> ExtendableDataDictionary::FieldDefByName(std::string field_name)
{
    auto it = field_def_by_name_.find(field_name);
    if ( it == field_def_by_name_.end())
    {
        return {};
    }
    else
    {
        return it->second;
    }
}

void ExtendableDataDictionary::AddFieldDef(FieldDef field_def)
{
    if (!DataDictionaryUtils::IsValid(field_def))
    {
        throw std::invalid_argument("invalid field def added to dictionary");
    }

    if (!field_defs_by_fid_.empty())
    {
        std::optional<FieldDef> previous = FieldDefByFid(field_def.fid_);
        if (previous)
        {
            HandleDuplicatedFid(*previous);
        }
    }
    field_defs_by_fid_.emplace(field_def.fid_, field_def);
    if (!field_def_by_name_.empty())
    {
        std::optional<FieldDef> previous = FieldDefByName(field_def.name_);
        if (previous)
        {
            HandleDuplicatedName(*previous);
        }
    }
    field_def_by_name_.emplace(field_def.name_, field_def);
}


void ExtendableDataDictionary::HandleDuplicatedFid(FieldDef& previous)
{
    field_defs_by_fid_.erase(previous.fid_);
    field_def_by_name_.erase(previous.name_);
}

void ExtendableDataDictionary::HandleDuplicatedName(FieldDef& previous)
{
    field_defs_by_fid_.erase(previous.fid_);
    field_def_by_name_.erase(previous.name_);
}

std::vector<FieldDef> ExtendableDataDictionary::AllFieldDefs()
{
    std::vector<FieldDef> all_fields_defs;
    for (auto const&[fid, field_def] : field_defs_by_fid_)
    {
        all_fields_defs.push_back(field_def);

    }
    return all_fields_defs;
}


int ExtendableDataDictionary::size()
{
    return static_cast<int>(field_def_by_name_.size());
}

int ExtendableDataDictionary::NextFreeFid()
{
    if (!field_defs_by_fid_.empty())
    {
        int largest = field_defs_by_fid_.rbegin()->first;
        return largest + 1;
    } else
    {
        return 0;
    }
}

} // namespace bidfx_public_api::price::pixie
