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

#ifndef PUBLIC_API_CPP_PUBLIC_API_H_
#define PUBLIC_API_CPP_PUBLIC_API_H_

#include <string>
#include <memory>
#include <include/session.h>

namespace bidfx_public_api
{

/**
 * This class is the main access point for the BidFX Public API, it provides basic API information and session creation.
 * @author Frazer Bennett Wilford
 */
using bidfx_public_api::Session;

class PublicApi
{

public:

    /**
     * Gets the API name.
     */
     static std::string GetName();

    /**
     * Gets the API version.
     */
    static std::string GetVersion();

    /**
     * Creates a new API session.
     */
    static std::unique_ptr<Session>  CreateSession();
};

}

#endif //PUBLIC_API_CPP_PUBLIC_API_H_
