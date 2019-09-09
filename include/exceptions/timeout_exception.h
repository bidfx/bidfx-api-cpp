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

#ifndef PUBLIC_API_CPP_EXCEPTIONS_TIMEOUT_EXCEPTION_H_
#define PUBLIC_API_CPP_EXCEPTIONS_TIMEOUT_EXCEPTION_H_

#include <string>
#include "include/exceptions/bid_fx_exception.h"

namespace bidfx_public_api::exceptions
{

/**
 * This exception is thrown when there is a timeout
 * @author Frazer Bennett Wilford
 */
class TimeoutException : public BidFXException
{
public:
    TimeoutException(std::string&& reason);
    TimeoutException(const std::string& reason);
    TimeoutException(const TimeoutException& to_copy);
    TimeoutException(TimeoutException&& to_move);

    TimeoutException& operator=(const TimeoutException& to_copy);
    TimeoutException& operator=(TimeoutException&& to_move);
};

}

#endif //PUBLIC_API_CPP_EXCEPTIONS_TIMEOUT_EXCEPTION_H_
