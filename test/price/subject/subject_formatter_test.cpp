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

#include "include/price/subject/subject_builder.h"
#include "src/price/subject/subject_formatter.h"
#include "include/price/subject/subject_utils.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::subject
{

using bidfx_public_api::price::subject::SubjectUtils;

SubjectFormatter FORMATTER = SubjectFormatter();

TEST(SubjectFormatterTest, test_creating_basic_subject_from_string)
{
    SubjectBuilder builder = SubjectBuilder();
    FORMATTER.ParseSubject("Symbol=EURGBP,Quantity=2500000", builder);

    Subject subject = builder.CreateSubject();
    ASSERT_EQ(subject.ToString(), "Quantity=2500000,Symbol=EURGBP");
}

}