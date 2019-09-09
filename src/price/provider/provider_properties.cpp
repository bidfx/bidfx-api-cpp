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
#include "include/price/provider/provider_properties.h"

namespace bidfx_public_api::price::provider
{

ProviderProperties::ProviderProperties(Provider::Status status, std::string name, std::string explanation)
{
    status_ = status;
    name_ = name;
    explanation_ = explanation;
}

/**
 * Gets the status.
 */
Provider::Status ProviderProperties::GetStatus()
{
    return status_;
}

std::string ProviderProperties::GetStatusString()
{
    return provider_status_map[status_];
}

/**
 * Gets the name of the provider.
 */
std::string ProviderProperties::GetName()
{
    return name_;
}

/**
 * Gets the event explanation text.
 */
std::string ProviderProperties::GetExplanation()
{
    return explanation_;
}

std::string ProviderProperties::Describe()
{
    std::stringstream ss;
    ss << name_ << " - " << status_ << " - " << explanation_;
    return ss.str();
}

}