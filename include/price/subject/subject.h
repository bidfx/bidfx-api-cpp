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

#ifndef PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_H_
#define PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_H_

#include <string>
#include <vector>
#include "spdlog/fmt/ostr.h"

namespace bidfx_public_api::price::subject
{
/**
 * This interface describes a subject that is used to identify information that may be subscribed to.
 * @author Frazer Bennett Wilford
 */
class Subject
{
private:
    std::vector<std::string> components_;
    friend std::ostream& operator<<(std::ostream& os, const Subject& s);

public:

    Subject();
    Subject(const Subject& to_copy);
    Subject(Subject&& to_move);
    Subject& operator=(const Subject& to_copy);
    Subject& operator=(Subject&& to_move);

    /**
     * Creates a subject from a vector of strings containing a keys followed by their value
     * example: {"Symbol","EURGBP","Quantity","2500000"} ..
     * It is recommended for ease of use that in most cases you use the SubjectFactory instead
     * @param components the vector contains keys and values for the subject to be created
     */
    Subject(std::vector<std::string> components);

    /**
     * Creates a subject using a formatted string of the form "Symbol=EURGBP,Quantity=2500000" ..
     * It is recommended for ease of use that in most cases you use the SubjectFactory instead
     * @param formatted_string contains a subject represented by "key=value,key=value" etc
     */
    Subject(const std::string& formatted_string);

    /**
     * Gets the value of a component from the subject.
     * @param key the component key.
     * @return the associated component value or null.
     */
    std::string GetComponent(std::string key) const;

    /**
     * Gets the components from the subject.
     * @return the component list
     */
    const std::vector<std::string>& GetComponents() const;

    /**
     * Converts the subject to a formatted subject string.
     */
    std::string ToString() const;

    int Compare(const Subject& that) const;

    bool operator==(const Subject& that) const;

    bool operator<(const Subject& that) const;
};

}

#endif //PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_H_