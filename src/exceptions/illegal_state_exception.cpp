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

#include "include/exceptions/illegal_state_exception.h"
 
namespace bidfx_public_api::exceptions
{

IllegalStateException::IllegalStateException(std::string&& reason) : BidFXException{std::move(reason)} {}
IllegalStateException::IllegalStateException(const std::string& reason) : BidFXException{reason} {}
IllegalStateException::IllegalStateException(const IllegalStateException& to_copy) : BidFXException{to_copy} {}
IllegalStateException::IllegalStateException(IllegalStateException&& to_move) : BidFXException{std::move(to_move)} {}

IllegalStateException& IllegalStateException::operator=(const IllegalStateException& to_copy)
{
    BidFXException::operator=(to_copy);
    return *this;
}

IllegalStateException& IllegalStateException::operator=(IllegalStateException&& to_move)
{
    BidFXException::operator=(std::move(to_move));
}

}