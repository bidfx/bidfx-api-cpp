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

#ifndef BIDFX_PUBLIC_API_SUBJECT_COMPARATOR_H_
#define BIDFX_PUBLIC_API_SUBJECT_COMPARATOR_H_

#include "include/price/subject/subject.h"

namespace bidfx_public_api::price::subject
{

class SubjectComparatorUtils
{
private:
    static const int BOTH_NON_NULL = 2;
    static int CompareStrings(const std::string& s1, const std::string& s2);
    static int CompareQuantity(const std::string& s1, const std::string& s2);
    static int CompareObject(const std::string& s1, const std::string& s2);
public:
    static int Compare(const Subject& a, const Subject& b);
};

struct
{
    bool operator()(const Subject& a, const Subject& b)
    {
        return SubjectComparatorUtils::Compare(a, b) < 0;
    }
} SubjectComparator;

}

#endif //BIDFX_PUBLIC_API_SUBJECT_COMPARATOR_H_
