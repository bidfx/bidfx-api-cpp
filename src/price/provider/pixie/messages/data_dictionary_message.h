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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_DATA_DICTIONARY_MESSAGE_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_DATA_DICTIONARY_MESSAGE_H_

#include <vector>
#include <memory>
#include "src/price/provider/pixie/fields/field_def.h"
#include "pixie_message.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::ByteBuffer;

/**
 * This class describes a Pixie data dictionary message.
 * @author Frazer Bennett Wilford
 */
class DataDictionaryMessage
{
private:
    int options_;
    std::vector<FieldDef> field_defs_;

    std::vector<FieldDef> FilterUnwantedFields(std::vector<FieldDef> fields) const;
    FieldDef ReadFieldDef(ByteBuffer& buffer);

public:
    DataDictionaryMessage(std::vector<FieldDef> field_defs, bool update);
    DataDictionaryMessage(ByteBuffer& buffer);

    const std::vector<FieldDef> GetFieldDefs() const;

    bool IsUpdate() const;
    const std::string ToString() const;
    bool operator==(const DataDictionaryMessage& that) const;
    bool operator!=(const DataDictionaryMessage& that) const;
};

inline std::ostream& operator<<(std::ostream& str, DataDictionaryMessage& v)
{
    return str << v.ToString();
}

}
#endif //PUBLIC_API_CPP_PRICE_PROVIDER_DATA_DICTIONARY_MESSAGE_H_
