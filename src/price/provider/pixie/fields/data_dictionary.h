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

#ifndef PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_DATA_DICTIONARY_H_
#define PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_DATA_DICTIONARY_H_

#include <vector>
#include <memory>
#include "field_def.h"
#include "src/tools/optional.h"

namespace bidfx_public_api::price::pixie
{

class IDataDictionary
{
public:
    virtual std::optional<FieldDef> FieldDefByFid(int fid) = 0;

    virtual std::optional<FieldDef> FieldDefByName(std::string field_name) = 0;

    virtual void AddFieldDef(FieldDef field_def) = 0;

    virtual std::vector<FieldDef> AllFieldDefs() = 0;

    virtual int size() = 0;

    virtual int NextFreeFid() = 0;

};

class NullDataDictionary : public IDataDictionary
{
private:
    NullDataDictionary() {}
public:
    static NullDataDictionary& getInstance()
    {
        static NullDataDictionary instance;
        return instance;
    }
    NullDataDictionary(NullDataDictionary const&) = delete;
    void operator=(NullDataDictionary const&) = delete;

    std::optional<FieldDef> FieldDefByFid(int fid) override
    {
        return {};
    }

    std::optional<FieldDef> FieldDefByName(std::string field_name) override
    {
        return {};
    }

    void AddFieldDef(FieldDef field_def) override
    {

    }

    std::vector<FieldDef> AllFieldDefs() override
    {
        return std::vector<FieldDef>();
    }

    int size() override
    {
        return 0;
    }

    int NextFreeFid() override
    {
        return 0;
    }
};

} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_FIELDS_DATA_DICTIONARY_H_
