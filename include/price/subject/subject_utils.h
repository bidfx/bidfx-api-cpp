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

#ifndef PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_UTILS_H_
#define PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_UTILS_H_

#include <vector>
#include <string>
#include "subject.h"

namespace bidfx_public_api::price::subject
{

/**
 * This class provides utility methods for handling subjects.
 * @author Frazer Bennett Wilford
 */
class SubjectUtils
{
public:
    /**
     * Performs a binary search for the position of a key in a component key-value pair list.
     * @param components the components: key, value, key, value, and so on.
     * @param size the number of items in the array.
     * @param key the key to search for.
     * @return the index of the key in the array or -1 - insertpoint if not found.
     */
    static int BinarySearch(const std::vector<std::string>* components, const std::string& key);

    /**
     * Creates a new subject from a formatted string of components
     * @param formatted_subject the string which contains the components to have a subject formed from
     * @return the subject which is created
     */
    static Subject CreateSubjectFromString(const std::string& formatted_subject);
};

}

#endif //PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_UTILS_H_
