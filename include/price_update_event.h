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

#ifndef PUBLIC_API_CPP_PRICE_UPDATE_EVENT_H_
#define PUBLIC_API_CPP_PRICE_UPDATE_EVENT_H_

#include <include/price/subject/subject.h>
#include <include/price/price_field.h>
#include <map>

namespace bidfx_public_api
{

using bidfx_public_api::price::subject::Subject;
using bidfx_public_api::price::PriceField;

/**
 * This interface represents a price update event.
 * @author Frazer Bennett Wilford
 */
class PriceUpdateEvent
{
private:
    Subject subject_;
    std::map<std::string, PriceField> allPriceFields_;
    std::map<std::string, PriceField> changedPriceFields_;
public:
    PriceUpdateEvent(Subject subject, std::map<std::string, PriceField> allPriceFields, std::map<std::string, PriceField> changedPriceFields);

    /**
     * Gets the subject of the subscription.
     */
    const Subject& GetSubject() const;

    /**
     * Gets the changed price fields in this update.
     */
    const std::map<std::string, PriceField>& GetChangedPriceFields() const;

    /**
     * Gets all of the price fields including the recent updates.
     */
    const std::map<std::string, PriceField>& GetAllPriceFields() const;
};

} // bidfx_public_api

#endif //PUBLIC_API_CPP_PRICE_UPDATE_EVENT_H_
