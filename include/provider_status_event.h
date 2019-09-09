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

#ifndef PUBLIC_API_CPP_PROVIDER_STATUS_EVENT_H_
#define PUBLIC_API_CPP_PROVIDER_STATUS_EVENT_H_

#include <include/price/subject/subject.h>
#include <include/price/provider/provider_properties.h>

namespace bidfx_public_api
{

using bidfx_public_api::price::subject::Subject;
using bidfx_public_api::price::provider::ProviderProperties;

/**
 * This interface defines price provider status change event.
 * @author Frazer Bennett Wilford
 */
class ProviderStatusEvent
{
public:
    /**
     * Gets the provider properties.
     */
    virtual ProviderProperties& GetProviderProperties() = 0;

    /**
     * Describes the event.
     */
    std::string Describe()
    {
        return GetProviderProperties().Describe();
    }
};

} // bidfx_public_api

#endif //PUBLIC_API_CPP_PROVIDER_STATUS_EVENT_H_
