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

#include "include/price/subject/subject.h"
#include "include/price/subject/subject_utils.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::subject
{

TEST(SubjectTest, test_compare_same_subject_should_be_0)
{
    Subject s1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1230000.00");
    Subject s2 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1230000.00");
    EXPECT_EQ(0, s1.Compare(s2));
}

TEST(SubjectTest, test_compare_same_all_but_one_extra_field_on_one_subject)
{
    Subject s1 = SubjectUtils::CreateSubjectFromString("Currency=GBP,Symbol=EURGBP");
    Subject s2 = SubjectUtils::CreateSubjectFromString("Currency=GBP,Symbol=EURGBP,Quantity=1230000.00");
    EXPECT_GT(s1.Compare(s2), 0);
    EXPECT_LT(s2.Compare(s1), 0 );
}

TEST(SubjectTest, test_compare_symbol_is_less_than)
{
    Subject s1 = SubjectUtils::CreateSubjectFromString("Currency=GBP,Symbol=EURGBP");
    Subject s2 = SubjectUtils::CreateSubjectFromString("Currency=GBP,Symbol=BAKGBP");
    EXPECT_GT(s1.Compare(s2), 0);
    EXPECT_LT(s2.Compare(s1), 0);
}

TEST(SubjectTest, testCreateFromString)
{
    Subject s1("Currency=EUR,Symbol=EURCHF,Quantity=2000000.00");
    Subject s2 = SubjectUtils::CreateSubjectFromString("Currency=EUR,Symbol=EURCHF,Quantity=2000000.00");
    EXPECT_EQ(s1, s2);
}

}
