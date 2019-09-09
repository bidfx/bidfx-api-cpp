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

#ifndef PUBLIC_API_CPP_EXCEPTIONS_BIDFX_EXCEPTION_H_
#define PUBLIC_API_CPP_EXCEPTIONS_BIDFX_EXCEPTION_H_

#include <string>

namespace bidfx_public_api::exceptions
{

/**
 * The base bidfx exception, used for custom exceptions
 * @author Frazer Bennett Wilford
 */
class BidFXException : public std::exception
{
private:
    std::string reason_;

public:
    BidFXException();
    virtual ~BidFXException() {};
    BidFXException(std::string&& reason);
    BidFXException(const std::string& reason);
    BidFXException(const BidFXException& to_copy);
    BidFXException(BidFXException&& to_move);

    BidFXException& operator=(const BidFXException& to_copy);
    BidFXException& operator=(BidFXException&& to_move);

    virtual const char* what() const throw()
    {
        return reason_.c_str();
    }

};

}

#endif //PUBLIC_API_CPP_EXCEPTIONS_BIDFX_EXCEPTION_H_
