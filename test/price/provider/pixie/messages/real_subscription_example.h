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

#ifndef BIDFX_PUBLIC_API_REAL_SUBSCRIPTION_EXAMPLE_H_
#define BIDFX_PUBLIC_API_REAL_SUBSCRIPTION_EXAMPLE_H_

#include "include/price/subject/subject.h"
#include "include/price/subject/subject_utils.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::subject::Subject;
using bidfx_public_api::price::subject::SubjectUtils;

class RealSubscriptionExample
{
public:
    static std::vector<Subject> REAL_SORTED_LIST;
};

}

#endif //BIDFX_PUBLIC_API_REAL_SUBSCRIPTION_EXAMPLE_H_
