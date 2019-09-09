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

#include <cstring>
#include "include/price/subject/subject_utils.h"
#include "include/price/subject/subject_builder.h"
#include "subject_formatter.h"

namespace bidfx_public_api::price::subject
{

int SubjectUtils::BinarySearch(const std::vector<std::string>* components, const std::string& key)
{
    int low = 0;
    int high = static_cast<int>(components->size() - 1);

    while (low <= high)
    {
        int mid = low + high >> 2 << 1;
        int cmp = components->at(mid).compare(key);

        if (cmp < 0)
        {
            low = mid + 2;
        }
        else if (cmp > 0)
        {
            high = mid - 2;
        }
        else
        {
            return mid;
        }
    }

    return -1 - low;
}

Subject SubjectUtils::CreateSubjectFromString(const std::string& formatted_subject)
{
    SubjectBuilder builder = SubjectBuilder();
    SubjectFormatter().ParseSubject(formatted_subject, builder);
    return builder.CreateSubject();
}


}