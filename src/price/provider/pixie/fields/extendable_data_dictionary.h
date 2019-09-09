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

#ifndef PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_EXTENDABLE_DATA_DICTIONARY_UTILS_H_
#define PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_EXTENDABLE_DATA_DICTIONARY_UTILS_H_

#include <map>
#include <vector>
#include <memory>
#include "data_dictionary.h"

namespace bidfx_public_api::price::pixie
{

class ExtendableDataDictionary : public IDataDictionary
{
private:

    std::map<int, FieldDef> field_defs_by_fid_ = std::map<int, FieldDef>();

    std::map<std::string, FieldDef> field_def_by_name_ = std::map<std::string, FieldDef>();

    void HandleDuplicatedFid(FieldDef& previous);

    void HandleDuplicatedName(FieldDef& previous);

public:
    ExtendableDataDictionary()
    {}

    std::optional<FieldDef> FieldDefByFid(int fid);

    std::optional<FieldDef> FieldDefByName(std::string field_name);

    void AddFieldDef(FieldDef field_def);

    std::vector<FieldDef> AllFieldDefs();

    int size();

    int NextFreeFid();
};

}

#endif //PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_EXTENDABLE_DATA_DICTIONARY_UTILS_H_
