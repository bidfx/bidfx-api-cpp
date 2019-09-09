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

#ifndef PUBLIC_API_CPP_PRICE_SUBJECT_INVALID_SUBJECT_EXCEPTION_H_
#define PUBLIC_API_CPP_PRICE_SUBJECT_INVALID_SUBJECT_EXCEPTION_H_

#include <string>

namespace bidfx_public_api::price::subject
{

/**
 * This exception is thrown when there are issues with the creation of a subject
 * @author Frazer Bennett Wilford
 */
class InvalidSubjectException : public std::exception
{
private:
    std::string reason_;

public:
    InvalidSubjectException(std::string&& reason);
    InvalidSubjectException(const InvalidSubjectException& to_copy);
    InvalidSubjectException(InvalidSubjectException&& to_move);

    InvalidSubjectException& operator=(const InvalidSubjectException& to_copy);
    InvalidSubjectException& operator=(InvalidSubjectException&& to_move);

    virtual const char* what() const throw()
    {
        return reason_.c_str();
    }
};


}

#endif //PUBLIC_API_CPP_PRICE_SUBJECT_INVALID_SUBJECT_EXCEPTION_H_