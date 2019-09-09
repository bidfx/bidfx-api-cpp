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

#include <random>
#include "include/price/subject/subject.h"
#include "include/price/subject/subject_utils.h"
#include "src/price/subject/subject_comparator.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::subject
{

TEST(SubjectComparatorTest, test_identical_subjects)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000.00,Account=AAA");
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(subject1, subject1));
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000.00,Account=AAA"), subject1));
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(subject1, SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000.00,Account=AAA")));
}

TEST(SubjectComparatorTest, test_not_equal_subjects_with_equivalent_quantity_and_symbol)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000.00,Account=AAA");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000");
    EXPECT_LT(SubjectComparatorUtils::Compare(subject1, subject2), 0);
    EXPECT_GT(SubjectComparatorUtils::Compare(subject2, subject1), 0);
}

TEST(SubjectComparatorTest, test_identical_subjects_with_equivalent_quantities)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000.00,Account=AAA");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000,Account=BBB");
    EXPECT_LT(SubjectComparatorUtils::Compare(subject1, subject2), 0);
    EXPECT_GT(SubjectComparatorUtils::Compare(subject2, subject1), 0);
}

TEST(SubjectComparatorTest, test_compare_sorts_first_by_symbol)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000.00,Account=ZZZ");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Symbol=GBPUSD,Quantity=2000000.00,Account=BBB");
    EXPECT_LT(SubjectComparatorUtils::Compare(subject1, subject2), 0);
    EXPECT_GT(SubjectComparatorUtils::Compare(subject2, subject1), 0);
}

TEST(SubjectComparatorTest, test_compare_sorts_second_by_quantity)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000,Account=ZZZ");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=2000000,Account=BBB");
    EXPECT_LT(SubjectComparatorUtils::Compare(subject1, subject2), 0);
    EXPECT_GT(SubjectComparatorUtils::Compare(subject2, subject1), 0);
}

TEST(SubjectComparatorTest, test_compare_sorts_by_quantity_down_to_the_penny)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=2000000.02,Account=AAA");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=2000000.03,Account=AAA");
    EXPECT_LT(SubjectComparatorUtils::Compare(subject1, subject2), 0);
    EXPECT_GT(SubjectComparatorUtils::Compare(subject2, subject1), 0);
}

TEST(SubjectComparatorTest, test_compare_sorts_by_quantity_even_when_they_differ_in_length)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=20000.00,Account=AAA");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000.00,Account=AAA");
    EXPECT_LT(SubjectComparatorUtils::Compare(subject1, subject2), 0);
    EXPECT_GT(SubjectComparatorUtils::Compare(subject2, subject1), 0);
}

TEST(SubjectComparatorTest, test_compare_sorts_by_quantity_even_if_first_is_not_in_double_format)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000,Account=AAA");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=2000000.00,Account=AAA");
    EXPECT_LT(SubjectComparatorUtils::Compare(subject1, subject2), 0);
    EXPECT_GT(SubjectComparatorUtils::Compare(subject2, subject1), 0);
}

TEST(SubjectComparatorTest, test_compare_sorts_by_quantity_even_if_neither_are_in_double_format)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1000000,Account=AAA");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=2000000,Account=AAA");
    EXPECT_LT(SubjectComparatorUtils::Compare(subject1, subject2), 0);
    EXPECT_GT(SubjectComparatorUtils::Compare(subject2, subject1), 0);
}

TEST(SubjectComparatorTest, test_compare_sorts_by_quantity_even_if_not_a_number)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=A1000000,Account=AAA");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=A2000000,Account=AAA");
    EXPECT_LT(SubjectComparatorUtils::Compare(subject1, subject2), 0);
    EXPECT_GT(SubjectComparatorUtils::Compare(subject2, subject1), 0);
}

TEST(SubjectComparatorTest, test_compare_third_by_overall_string)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=2000000.00,Account=AAA");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=2000000.00,Account=BBB");
    EXPECT_LT(SubjectComparatorUtils::Compare(subject1, subject2), 0);
    EXPECT_GT(SubjectComparatorUtils::Compare(subject2, subject1), 0);
}

TEST(SubjectComparatorTest, test_compare_identical_with_missing_symbol)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Quantity=2000000.00,Account=AAA");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Quantity=2000000.00,Account=AAA");
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(subject1, subject2));
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(subject2, subject1));
}

TEST(SubjectComparatorTest, test_compare_identical_with_missing_symbol_and_account)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Quantity=2000000.00");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Quantity=2000000.00");
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(subject1, subject2));
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(subject2, subject1));
}

TEST(SubjectComparatorTest, test_compare_identical_with_missing_symbol_and_quantity)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Account=AAA");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Account=AAA");
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(subject1, subject2));
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(subject2, subject1));
}

TEST(SubjectComparatorTest, test_compare_identical_but_different_order)
{
    Subject subject1 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1230000.00");
    Subject subject2 = SubjectUtils::CreateSubjectFromString("Quantity=1230000.00,Symbol=EURGBP");
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(subject1, subject2));
    EXPECT_EQ(0, SubjectComparatorUtils::Compare(subject2, subject1));
}

}