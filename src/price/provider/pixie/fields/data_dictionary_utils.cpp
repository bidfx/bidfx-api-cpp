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

#include <memory>
#include "data_dictionary_utils.h"

namespace bidfx_public_api::price::pixie
{

bool DataDictionaryUtils::IsValid(FieldDef field_def)
{
    return field_def.fid_ >= 0 &&
          !field_def.name_.empty() &&
           field_def.encoding_ != FieldEncodingEnum::NOT_DEFINED;
}

void DataDictionaryUtils::AddAllFields(IDataDictionary& data_dictionary,
                                       std::vector<FieldDef>& field_defs)
{
    for (const auto& curr_field_def : field_defs)
    {
        data_dictionary.AddFieldDef(curr_field_def);
    }
}

} // namespace bidfx_public_api::price::pixie