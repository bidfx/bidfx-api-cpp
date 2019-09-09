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

#include "subject_comparator.h"
#include "subject_key.h"

namespace bidfx_public_api::price::subject
{

int SubjectComparatorUtils::Compare(const Subject& a, const Subject& b)
{
    int cmp = SubjectComparatorUtils::CompareStrings(a.GetComponent(SubjectKey::SYMBOL),
                                                     b.GetComponent(SubjectKey::SYMBOL));

    if (cmp == 0)
    {
        cmp = SubjectComparatorUtils::CompareQuantity(a.GetComponent(SubjectKey::QUANTITY),
                                                      b.GetComponent(SubjectKey::QUANTITY));
        if (cmp == 0)
        {
            return a.Compare(b);
        }
    }

    return cmp;
}

int SubjectComparatorUtils::CompareQuantity(const std::string& s1, const std::string& s2)
{
    int cmp = CompareObject(s1, s2);
    if (cmp != BOTH_NON_NULL) return cmp;
    try
    {
        double val1 = std::stod(s1);
        double val2 = std::stod(s2);

        if (val1 < val2) return -1;
        return val1 == val2 ? 0 : 1;
    }
    catch (std::exception e)
    {
        return s1.compare(s2);
    }
}

int SubjectComparatorUtils::CompareStrings(const std::string& s1, const std::string& s2)
{
    int cmp = CompareObject(s1, s2);
    return cmp == BOTH_NON_NULL ? s1.compare(s2) : cmp;
}

int SubjectComparatorUtils::CompareObject(const std::string& s1, const std::string& s2)
{
    if (s1 == s2) return 0;
    if (s1.empty()) return -1;
    if (s2.empty()) return 1;
    return SubjectComparatorUtils::BOTH_NON_NULL;
}

}
