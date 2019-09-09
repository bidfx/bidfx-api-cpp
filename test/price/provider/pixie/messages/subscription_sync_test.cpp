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
#include "include/price/subject/subject.h"
#include "include/price/subject/subject_utils.h"
#include "src/price/provider/pixie/messages/subscription_sync.h"
#include "src/price/provider/pixie/pixie_version.h"
#include "src/tools/buffer_util.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie::subscription_sync_test
{

using bidfx_public_api::price::subject::Subject;
using bidfx_public_api::price::subject::SubjectUtils;
using bidfx_public_api::tools::BufferUtil;
using namespace std::string_literals;

static const int EDITION = 123;
static const Subject REAL_SUBJECT = SubjectUtils::CreateSubjectFromString("Account=AXA_ACCT1,AssetClass=Fx,Currency=GBP,Customer=0001,Dealer=101519,Exchange=OTC,Level=1,Quantity=2000000.00,QuoteStyle=RFS,Source=SGFX,SubClass=Spot,Symbol=GBPUSD,Tenor=Spot,User=jsampson,ValueDate=20141103");

std::vector<Subject> subject_list = {SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=2500000"),
                                                      SubjectUtils::CreateSubjectFromString("Symbol=USDJPY,Quantity=5600000")};

TEST(SubscriptionSyncTest, test_is_changed_is_true_by_default)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    ASSERT_TRUE(subscription_sync.IsChangedEdition());
}

TEST(SubscriptionSyncTest, test_size_is_populated_from_the_subject_list_size)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    ASSERT_EQ(subject_list.size(), subscription_sync.GetSize());
}

TEST(SubscriptionSyncTest, test_is_compressed_false_by_default)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    ASSERT_FALSE(subscription_sync.IsCompressed());
}

TEST(SubscriptionSyncTest, test_has_no_controls_by_default)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    ASSERT_FALSE(subscription_sync.HasControls());
}

TEST(SubscriptionSyncTest, test_setting_valid_controls_has_controls_is_true)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    subscription_sync.AddControl(0, ControlOperationEnum::TOGGLE);
    ASSERT_TRUE(subscription_sync.HasControls());
}

TEST(SubscriptionSyncTest, test_bugfix_has_controls_remains_true_after_setting_compression)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    subscription_sync.AddControl(0, ControlOperationEnum::TOGGLE);
    ASSERT_TRUE(subscription_sync.HasControls());
    subscription_sync.SetCompressed(true);
    ASSERT_TRUE(subscription_sync.HasControls());
}

TEST(SubscriptionSyncTest, test_summarize)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    ASSERT_EQ("SubscriptionSync(edition=123, compressed=false, controls=0, changed=true, subjects=2)",
                 subscription_sync.Summarize());

    subscription_sync.AddControl(0, ControlOperationEnum::TOGGLE);
    subscription_sync.AddControl(1, ControlOperationEnum::REFRESH);
    ASSERT_EQ("SubscriptionSync(edition=123, compressed=false, controls=2, changed=true, subjects=2)",
                 subscription_sync.Summarize());

    subscription_sync.SetCompressed(true);
    ASSERT_EQ("SubscriptionSync(edition=123, compressed=true, controls=2, changed=true, subjects=2)",
                 subscription_sync.Summarize());
}

TEST(SubscriptionSyncTest, test_to_string)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    ASSERT_EQ("SubscriptionSync(edition=123, compressed=false, controls=0, changed=true, subjects=2) [\n"
                         "  Quantity=2500000,Symbol=EURGBP\n"
                         "  Quantity=5600000,Symbol=USDJPY\n"
                         "]", subscription_sync.ToString());
}

TEST(SubscriptionSyncTest, test_to_string_when_all_subjects_have_controls)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);

    subscription_sync.AddControl(0, ControlOperationEnum::TOGGLE);
    subscription_sync.AddControl(1, ControlOperationEnum::REFRESH);

    ASSERT_EQ("SubscriptionSync(edition=123, compressed=false, controls=2, changed=true, subjects=2) [\n"
                 "  Quantity=2500000,Symbol=EURGBP (TOGGLE)\n"
                 "  Quantity=5600000,Symbol=USDJPY (REFRESH)\n"
                 "]", subscription_sync.ToString());
}

TEST(SubscriptionSyncTest, test_to_string_with_a_few_controls_and_compressed)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);

    subscription_sync.SetCompressed(true);
    subscription_sync.AddControl(1, ControlOperationEnum::REFRESH);

    ASSERT_EQ("SubscriptionSync(edition=123, compressed=true, controls=1, changed=true, subjects=2) [\n"
                 "  Quantity=2500000,Symbol=EURGBP\n"
                 "  Quantity=5600000,Symbol=USDJPY (REFRESH)\n"
                 "]", subscription_sync.ToString());
}

TEST(SubscriptionSyncTest, test_encode_without_controls)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    ByteBuffer buffer = subscription_sync.Encode(PixieVersion::CURRENT_VERSION);
    ASSERT_EQ("S\000\173\002\004\011Quantity\0102500000\007Symbol\007EURGBP\004\011Quantity\0105600000\007Symbol\007USDJPY"s, BufferUtil::BufferAsString(buffer));
}

TEST(SubscriptionSyncTest, test_encode_with_controls)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    subscription_sync.AddControl(0, ControlOperationEnum::REFRESH);
    subscription_sync.AddControl(1, ControlOperationEnum::TOGGLE);

    ByteBuffer buffer = subscription_sync.Encode(PixieVersion::CURRENT_VERSION);
    ASSERT_EQ("S\002\173\002\004\011Quantity\0102500000\007Symbol\007EURGBP\004\011Quantity\0105600000\007Symbol\007USDJPY\002\000R\001T"s, BufferUtil::BufferAsString(buffer));
}

TEST(SubscriptionSyncTest, test_encode_with_unchanged_edition_with_controls)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    subscription_sync.AddControl(0, ControlOperationEnum::REFRESH);
    subscription_sync.AddControl(1, ControlOperationEnum::TOGGLE);
    subscription_sync.SetChangedEdition(false);

    ByteBuffer buffer = subscription_sync.Encode(PixieVersion::CURRENT_VERSION);
    ASSERT_EQ("S\006\173\002\002\000R\001T"s, BufferUtil::BufferAsString(buffer));
}

TEST(SubscriptionSyncTest, test_encode_with_controls_version1_controls_are_omitted)
{
    SubscriptionSync subscription_sync = SubscriptionSync(EDITION, subject_list);
    subscription_sync.AddControl(0, ControlOperationEnum::REFRESH);
    subscription_sync.AddControl(1, ControlOperationEnum::TOGGLE);

    ByteBuffer buffer = subscription_sync.Encode(PixieVersion::VERSION1);
    ASSERT_EQ("S\000\173\002\004\011Quantity\0102500000\007Symbol\007EURGBP\004\011Quantity\0105600000\007Symbol\007USDJPY"s, BufferUtil::BufferAsString(buffer));
}

}