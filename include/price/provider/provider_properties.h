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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PROVIDER_PROPERTIES_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PROVIDER_PROPERTIES_H_

#include "include/price/provider/provider.h"

namespace bidfx_public_api::price::provider
{

/**
 * @author Frazer Bennett Wilford
 */
class ProviderProperties
{
private:
    std::string name_;
    Provider::Status status_;
    std::string explanation_;

public:
    ProviderProperties(Provider::Status status, std::string name, std::string explanation);

    /**
     * Gets the status.
     */
    Provider::Status GetStatus();

    /**
     * Gets the string representation of the status
     */
    std::string GetStatusString();

    /**
     * Gets the name of the provider.
     */
    std::string GetName();

    /**
     * Gets the event explanation text.
     */
    std::string GetExplanation();

    /**
     * Describes the event.
     */
     std::string Describe();
};

}

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PROVIDER_PROPERTIES_H_
