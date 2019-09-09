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

#include <string>
#include "include/price/subject/subject_builder.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::subject
{

TEST(SubjectBuilderTest, test_get_component)
{
    SubjectBuilder subject_builder = SubjectBuilder();

    subject_builder.SetComponent("A", "1").SetComponent("B", "2").SetComponent("C", "3").SetComponent("D", "4");
    EXPECT_STREQ(subject_builder.GetComponent("A").c_str(), "1");
    EXPECT_STREQ(subject_builder.GetComponent("B").c_str(), "2");
    EXPECT_STREQ(subject_builder.GetComponent("C").c_str(), "3");
    EXPECT_STREQ(subject_builder.GetComponent("D").c_str(), "4");
}

TEST(SubjectBuilderTest, test_a_typical_subject)
{
    SubjectBuilder subject_builder = SubjectBuilder();

    subject_builder
            .SetComponent("Level", "1")
            .SetComponent("Symbol", "VOD.L")
            .SetComponent("Source", "Reuters")
            .SetComponent("AssetClass", "Equity")
            .SetComponent("Exchange", "LSE");

    EXPECT_STREQ(subject_builder.GetComponent("Level").c_str(), "1");
    EXPECT_STREQ(subject_builder.GetComponent("Symbol").c_str(), "VOD.L");
    EXPECT_STREQ(subject_builder.GetComponent("Source").c_str(), "Reuters");
    EXPECT_STREQ(subject_builder.GetComponent("AssetClass").c_str(), "Equity");
    EXPECT_STREQ(subject_builder.GetComponent("Exchange").c_str(), "LSE");
}

TEST(SubjectBuilderTest, test_component_keys_are_added_alphabetically)
{
    SubjectBuilder subject_builder = SubjectBuilder();

    subject_builder
            .SetComponent("Level", "1")
            .SetComponent("Symbol", "VOD.L")
            .SetComponent("Source", "Reuters")
            .SetComponent("AssetClass", "Equity");

    std::vector<std::string> output = subject_builder.GetComponents();

    EXPECT_STREQ(output.at(0).c_str(), "AssetClass");
    EXPECT_STREQ(output.at(2).c_str(), "Level");
    EXPECT_STREQ(output.at(4).c_str(), "Source");
    EXPECT_STREQ(output.at(6).c_str(), "Symbol");
}

TEST(SubjectBuilderTest, test_populated_builder_to_string)
{
    SubjectBuilder subject_builder = SubjectBuilder();

    subject_builder
            .SetComponent("Level", "1")
            .SetComponent("Symbol", "VOD.L")
            .SetComponent("Source", "Reuters")
            .SetComponent("AssetClass", "Equity");

    EXPECT_STREQ(subject_builder.ToString().c_str(), "AssetClass=Equity,Level=1,Source=Reuters,Symbol=VOD.L");
}

}