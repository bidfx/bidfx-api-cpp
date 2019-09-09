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

#include "include/exceptions/timeout_exception.h"

namespace bidfx_public_api::exceptions
{

TimeoutException::TimeoutException(std::string&& reason) : BidFXException{std::move(reason)} {}
TimeoutException::TimeoutException(const std::string& reason) : BidFXException{reason} {}
TimeoutException::TimeoutException(const TimeoutException& to_copy) : BidFXException{to_copy} {}
TimeoutException::TimeoutException(TimeoutException&& to_move) : BidFXException{std::move(to_move)} {}

TimeoutException& TimeoutException::operator=(const TimeoutException& to_copy)
{
    BidFXException::operator=(to_copy);
    return *this;
}

TimeoutException& TimeoutException::operator=(TimeoutException&& to_move)
{
    BidFXException::operator=(std::move(to_move));
    return *this;
}

}