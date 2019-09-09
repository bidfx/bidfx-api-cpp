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

#ifndef PUBLIC_API_CPP_EXCEPTIONS_ILLEGAL_STATE_EXCEPTION_H_
#define PUBLIC_API_CPP_EXCEPTIONS_ILLEGAL_STATE_EXCEPTION_H_

#include "include/exceptions/bid_fx_exception.h"

namespace bidfx_public_api::exceptions
{

/**
 * This exception is thrown when there is an illegal state issue
 * @author Frazer Bennett Wilford
 */
class IllegalStateException : public BidFXException
{

public:
    IllegalStateException(std::string&& reason);
    IllegalStateException(const std::string& reason);
    IllegalStateException(const IllegalStateException& to_copy);
    IllegalStateException(IllegalStateException&& to_move);

    IllegalStateException& operator=(const IllegalStateException& to_copy);
    IllegalStateException& operator=(IllegalStateException&& to_move);
};

}

#endif //PUBLIC_API_CPP_EXCEPTIONS_ILLEGAL_STATE_EXCEPTION_H_
