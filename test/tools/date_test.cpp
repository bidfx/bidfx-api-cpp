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

#include "include/tools/date.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::tools {

TEST(DateTest, test_valid_date_to_string) {
    Date* date1 = new Date(2018, 10, 10);
    ASSERT_STREQ(date1->ToString().c_str(), "20181010");

    Date* date2 = new Date(2018, 9, 5);
    ASSERT_STREQ(date2->ToString().c_str(), "20180905");
};

}
