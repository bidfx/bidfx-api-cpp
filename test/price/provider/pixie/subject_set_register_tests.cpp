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
#include "src/price/provider/pixie/subject_set_register.h"
#include "test/price/provider/pixie/messages/real_subscription_example.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::subject::Subject;
using bidfx_public_api::price::subject::SubjectUtils;
using bidfx_public_api::price::pixie::FieldDef;
using bidfx_public_api::price::pixie::FieldTypeEnum;

const Subject EURCHR_2MM = SubjectUtils::CreateSubjectFromString(
        "SellSideAccount=DYMON,SellSideAllocAccount=DYMONCITI,AllocQuantity=2000000,AssetClass=Fx,Currency=EUR"
        ",BuySideId=1A78668,Level=1,NumAllocs=1"
        ",Symbol=EURCHF,Quantity=2000000.00"
        ",RequestFor=Stream,LiquidityProvider=JEFFX,DealType=Spot,Tenor=Spot,User=cheechungli,SettlementDate=20141114");
const Subject EURGBP_1MM = SubjectUtils::CreateSubjectFromString(
        "SellSideAccount=CIC,SellSideAllocAccount=CIC,AllocQuantity=1000000,AssetClass=Fx,Currency=EUR"
        ",BuySideId=1A78668,Level=1,NumAllocs=1"
        ",Symbol=EURGBP,Quantity=1000000.00"
        ",RequestFor=Stream,LiquidityProvider=MSFX,DealType=Spot,Tenor=Spot,User=brucelee,SettlementDate=20141109");
const Subject EURGBP_5MM = SubjectUtils::CreateSubjectFromString(
        "SellSideAccount=CIC,SellSideAllocAccount=CIC,AllocQuantity=5000000,AssetClass=Fx,Currency=EUR"
        ",BuySideId=1A78668,Level=1,NumAllocs=1"
        ",Symbol=EURGBP,Quantity=5000000.00"
        ",RequestFor=Stream,LiquidityProvider=MSFX,DealType=Spot,Tenor=Spot,User=brucelee,SettlementDate=20141107");
const Subject GBPUSD_1MM = SubjectUtils::CreateSubjectFromString(
        "SellSideAccount=AAA,SellSideAllocAccount=AAA,AllocQuantity=1000000,AssetClass=Fx,Currency=USD"
        ",BuySideId=1A78668,Level=1,NumAllocs=1"
        ",Symbol=GBPUSD,Quantity=1000000.00"
        ",RequestFor=Quote,LiquidityProvider=MSFX,DealType=Spot,Tenor=Spot,User=wchurchill,SettlementDate=20141105");

const Subject SS_SUBJECT0 = EURCHR_2MM;
const Subject SS_SUBJECT1 = EURGBP_1MM;
const Subject SS_SUBJECT2 = EURGBP_5MM;
const Subject SS_SUBJECT3 = GBPUSD_1MM;
const Subject SS_RFS_SUBJECT1 = SS_SUBJECT1;
const Subject SS_RFQ_SUBJECT3 = SS_SUBJECT3;

const FieldDef FIELD_DEF1(1, FieldTypeEnum::STRING, "field1");
const FieldDef FIELD_DEF2(1, FieldTypeEnum::STRING, "field2");

TEST(SubjectSetRegisterTest, initially_no_subjects_are_currently_subscribed)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    EXPECT_FALSE(subject_register.IsCurrentlySubscribed(SS_SUBJECT0));
    EXPECT_FALSE(subject_register.IsCurrentlySubscribed(SS_SUBJECT1));
    EXPECT_FALSE(subject_register.IsCurrentlySubscribed(SS_SUBJECT2));
}

TEST(SubjectSetRegisterTest, registering_a_subject_makes_it_currently_subscribed)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    EXPECT_TRUE(subject_register.IsCurrentlySubscribed(SS_SUBJECT0));
}

TEST(SubjectSetRegisterTest, registering_many_subjects_makes_them_currently_subscribed)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT2, false);
    EXPECT_TRUE(subject_register.IsCurrentlySubscribed(SS_SUBJECT0));
    EXPECT_TRUE(subject_register.IsCurrentlySubscribed(SS_SUBJECT1));
    EXPECT_TRUE(subject_register.IsCurrentlySubscribed(SS_SUBJECT2));
}

TEST(SubjectSetRegisterTest, unregistering_a_subject_makes_it_not_currently_subscribed)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT2, false);

    subject_register.Unregister(SS_SUBJECT0);
    subject_register.Unregister(SS_SUBJECT1);

    EXPECT_EQ(false, subject_register.IsCurrentlySubscribed(SS_SUBJECT0));
    EXPECT_EQ(false, subject_register.IsCurrentlySubscribed(SS_SUBJECT1));
    EXPECT_EQ(true, subject_register.IsCurrentlySubscribed(SS_SUBJECT2));

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    std::vector<Subject> set_comparison = {SS_SUBJECT2};
    EXPECT_EQ(set_comparison, message->GetSubjects());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_returns_null_when_there_have_been_no_registry_operations)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_returns_null_when_there_have_been_no_registry_operations_since_the_last_call)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.NextSubscriptionSync();
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_registering_subjects_provides_those_subjects)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    std::vector<Subject> set_comparison = {SS_SUBJECT0, SS_SUBJECT1};
    EXPECT_EQ(set_comparison, message->GetSubjects());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_registering_subjects_provides_the_latest_complete_set)
{
    SubjectSetRegister subject_register = SubjectSetRegister();

    subject_register.Register(SS_SUBJECT0, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    std::vector<Subject> set_comparison = {SS_SUBJECT0};
    EXPECT_EQ(set_comparison, message->GetSubjects());

    subject_register.Register(SS_SUBJECT1, false);
    message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    set_comparison = {SS_SUBJECT0, SS_SUBJECT1};
    EXPECT_EQ(set_comparison, message->GetSubjects());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_registering_then_unregistering_will_be_null)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Unregister(SS_SUBJECT0);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_returns_an_uncompressed_message_that_we_can_compress_later)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    EXPECT_EQ(false, message->IsCompressed());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_first_registration_has_edition)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    EXPECT_EQ(1, message->GetEdition());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_has_a_new_edition_number_after_each_change_involving_a_newly_registered_subject)
{
    SubjectSetRegister subject_register = SubjectSetRegister();

    subject_register.Register(SS_SUBJECT0, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    ASSERT_EQ(1, message->GetEdition());

    subject_register.Register(SS_SUBJECT1, false);
    message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    ASSERT_EQ(2, message->GetEdition());

    subject_register.Register(SS_SUBJECT2, false);
    message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    ASSERT_EQ(3, message->GetEdition());

    message = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_has_new_edition_number_after_each_change_involving_a_unregistered_subject)
{
    SubjectSetRegister subject_register = SubjectSetRegister();

    subject_register.Register(SS_SUBJECT0, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    ASSERT_EQ(1, message->GetEdition());

    subject_register.Unregister(SS_SUBJECT0);
    message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    ASSERT_EQ(2, message->GetEdition());

    message = subject_register.NextSubscriptionSync();
    ASSERT_FALSE(message);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_provides_the_registered_subjects_sorted_by_symbol_and_quantity)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT3, false);
    subject_register.Register(SS_SUBJECT1, false);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    std::vector<Subject> set_comparison = {SS_SUBJECT0, SS_SUBJECT1, SS_SUBJECT2, SS_SUBJECT3};
    EXPECT_EQ(set_comparison, message->GetSubjects());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_duplicate_non_refresh_registration_returns_null_as_nothing_has_changed)
{
    SubjectSetRegister subject_register = SubjectSetRegister();

    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    std::vector<Subject> set_comparison = {SS_SUBJECT0, SS_SUBJECT1};
    EXPECT_EQ(set_comparison, message->GetSubjects());

    subject_register.Register(SS_SUBJECT0, false);
    message = subject_register.NextSubscriptionSync();
    ASSERT_FALSE(message);

    subject_register.Register(SS_SUBJECT1, false);
    message = subject_register.NextSubscriptionSync();
    ASSERT_FALSE(message);

    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT1, false);
    message = subject_register.NextSubscriptionSync();
    ASSERT_FALSE(message);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_unregistering_all_subjects_gives_an_empty_subject_list)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT2, false);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    std::vector<Subject> set_comparison = {SS_SUBJECT0, SS_SUBJECT1, SS_SUBJECT2};
    EXPECT_EQ(set_comparison, message->GetSubjects());

    subject_register.Unregister(SS_SUBJECT0);
    subject_register.Unregister(SS_SUBJECT1);
    subject_register.Unregister(SS_SUBJECT2);

    message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    EXPECT_TRUE(message->GetSubjects().empty());
    EXPECT_TRUE(message->IsChangedEdition());
}

TEST(SubjectSetRegisterTest, test_subject_set_by_edition_returns_no_value_initially)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    EXPECT_FALSE(subject_register.SubjectSetByEdition(1));
    EXPECT_FALSE(subject_register.SubjectSetByEdition(2));
}

TEST(SubjectSetRegisterTest, next_subscription_sync_updates_the_subject_set_by_edition)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, false);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    auto subject_set = subject_register.SubjectSetByEdition(message->GetEdition());
    std::vector<Subject> set_comparison1 = {SS_SUBJECT0, SS_SUBJECT1};
    EXPECT_EQ(set_comparison1, *subject_set);

    subject_register.Register(SS_SUBJECT2, false);
    message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    subject_set = subject_register.SubjectSetByEdition(message->GetEdition());
    std::vector<Subject> set_comparison2 = {SS_SUBJECT0, SS_SUBJECT1, SS_SUBJECT2};
    EXPECT_EQ(set_comparison2, *subject_set);
}

TEST(SubjectSetRegisterTest, subject_set_by_edition_given_access_to_previous_subject_sets_are_archived_use_in_price_sync_handling)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    std::optional<SubscriptionSync> message1 = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message1);
    subject_register.Register(SS_SUBJECT1, false);
    std::optional<SubscriptionSync> message2 = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message2);
    subject_register.Register(SS_SUBJECT2, false);
    std::optional<SubscriptionSync> message3 = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message3);

    auto subject_set1 = subject_register.SubjectSetByEdition(message1->GetEdition());
    auto subject_set2 = subject_register.SubjectSetByEdition(message2->GetEdition());
    auto subject_set3 = subject_register.SubjectSetByEdition(message3->GetEdition());


    ASSERT_TRUE(subject_set1);
    ASSERT_TRUE(subject_set2);
    ASSERT_TRUE(subject_set3);

    EXPECT_EQ(message1->GetSubjects(), *subject_set1);
    EXPECT_EQ(message2->GetSubjects(), *subject_set2);
    EXPECT_EQ(message3->GetSubjects(), *subject_set3);
}

TEST(SubjectSetRegisterTest, subject_set_by_edition_removes_all_older_edition_subject_sets)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.NextSubscriptionSync();
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.NextSubscriptionSync();
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.NextSubscriptionSync();

    auto subject_set3 = subject_register.SubjectSetByEdition(3);
    EXPECT_FALSE(subject_register.SubjectSetByEdition(1));
    EXPECT_FALSE(subject_register.SubjectSetByEdition(2));
    EXPECT_EQ(*subject_set3, *subject_register.SubjectSetByEdition(3));
    EXPECT_EQ(*subject_set3, *subject_register.SubjectSetByEdition(3));
}

TEST(SubjectSetRegisterTest, subject_set_by_edition_gives_multiple_access_to_old_subject_set_editions)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.NextSubscriptionSync();
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.NextSubscriptionSync();
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.NextSubscriptionSync();

    auto subject_set1 = subject_register.SubjectSetByEdition(1);
    EXPECT_EQ(*subject_set1, *subject_register.SubjectSetByEdition(1));
    EXPECT_EQ(*subject_set1, *subject_register.SubjectSetByEdition(1));

    auto subject_set2 = subject_register.SubjectSetByEdition(2);
    EXPECT_EQ(*subject_set2, *subject_register.SubjectSetByEdition(2));
    EXPECT_EQ(*subject_set2, *subject_register.SubjectSetByEdition(2));
}

TEST(SubjectSetRegisterTest, test_subscribe_then_unsubscribe_quickly_this_should_never_result_in_a_subscription_sync)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, true);
    subject_register.Unregister(SS_SUBJECT0);
    subject_register.Unregister(SS_SUBJECT1);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message);

    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, true);
    subject_register.Unregister(SS_SUBJECT0);
    subject_register.Unregister(SS_SUBJECT1);

    message = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message);

    subject_register.Register(SS_SUBJECT2, false);
    subject_register.Unregister(SS_SUBJECT2);

    message = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message);
}

TEST(SubjectSetRegisterTest, test_unregister_works_with_different_object)
{
    SubjectSetRegister subject_register = SubjectSetRegister();

    const Subject subject1 = SubjectUtils::CreateSubjectFromString(
            "SellSideAccount=CIC,SellSideAllocAccount=CIC,AllocQuantity=1000000,AssetClass=Fx,Currency=EUR"
            ",BuySideId=1A78668,Level=1,NumAllocs=1"
            ",Symbol=EURGBP,Quantity=1000000.00"
            ",RequestFor=Stream,LiquidityProvider=MSFX,DealType=Spot,Tenor=Spot,User=brucelee,SettlementDate=20141109");

    const Subject subject2 = SubjectUtils::CreateSubjectFromString(
            "SellSideAccount=CIC,SellSideAllocAccount=CIC,AllocQuantity=1000000,AssetClass=Fx,Currency=EUR"
            ",BuySideId=1A78668,Level=1,NumAllocs=1"
            ",Symbol=EURGBP,Quantity=1000000.00"
            ",RequestFor=Stream,LiquidityProvider=MSFX,DealType=Spot,Tenor=Spot,User=brucelee,SettlementDate=20141109");


    EXPECT_FALSE(subject_register.IsCurrentlySubscribed(subject1));
    EXPECT_FALSE(subject_register.IsCurrentlySubscribed(subject2));
    subject_register.Register(subject1, false);
    EXPECT_TRUE(subject_register.IsCurrentlySubscribed(subject1));
    EXPECT_TRUE(subject_register.IsCurrentlySubscribed(subject2));
    subject_register.Unregister(subject2);
    EXPECT_FALSE(subject_register.IsCurrentlySubscribed(subject1));
    EXPECT_FALSE(subject_register.IsCurrentlySubscribed(subject2));
}

TEST(SubjectSetRegisterTest, test_comparator_with_messed_up_subjects)
{
    Subject s1 = SubjectUtils::CreateSubjectFromString("Account=DYMONCITI,Currency=EUR,Exchange=OTC");
    Subject s2 = SubjectUtils::CreateSubjectFromString("AllocAccount=DYMONCITI,Currency=EUR");
    Subject s3 = SubjectUtils::CreateSubjectFromString("Account=DYMONCITI,Currency=EUR,Exchange=OTC,Quantity=2000000.00");
    Subject s4 = SubjectUtils::CreateSubjectFromString("Symbol=EURCHF,Quantity=500000.00");
    Subject s5 = SubjectUtils::CreateSubjectFromString("Currency=EUR,Symbol=EURCHF,Quantity=2000000.00");
    Subject s6 = SubjectUtils::CreateSubjectFromString("Symbol=EURCHF,Quantity=2000000.00");
    Subject s7 = SubjectUtils::CreateSubjectFromString("Quantity=1230000.00,Symbol=EURGBP");
    Subject s8 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=1230000.00");
    Subject s9 = SubjectUtils::CreateSubjectFromString("Symbol=EURGBP,Quantity=5000000.00");
    Subject s10 = SubjectUtils::CreateSubjectFromString("Symbol=USDJPY,Account=DYMONCITI");

    std::vector<Subject> original_input = {s1, s2, s3, s4, s5, s6, s7, s9, s10};
    std::vector<Subject> shuffle_input = {s1, s2, s3, s4, s5, s6, s7, s8, s9, s10};
    auto rng = std::default_random_engine {};

    for (int i = 0; i < 100; i++)
    {
        SubjectSetRegister subject_register = SubjectSetRegister();
        std::shuffle(std::begin(shuffle_input), std::end(shuffle_input), rng);

        for (Subject subject : shuffle_input)
        {
            subject_register.Register(subject, false);
        }

        std::vector<Subject> output = subject_register.NextSubscriptionSync()->GetSubjects();

        EXPECT_EQ(original_input.size(), output.size());

        for (size_t j = 0; j < original_input.size(); j++)
        {
            EXPECT_EQ(original_input[j], output[j]);
        }
    }
}

TEST(SubjectSetRegisterTest, test_comparator_with_realistic_subjects)
{
    std::vector<Subject> shuffle_input(RealSubscriptionExample::REAL_SORTED_LIST);
    auto rng = std::default_random_engine {};

    for (int i = 0; i < 100; i++)
    {
        SubjectSetRegister subject_register = SubjectSetRegister();
        std::shuffle(std::begin(shuffle_input), std::end(shuffle_input), rng);

        for (Subject subject : shuffle_input)
        {
            subject_register.Register(std::move(subject), false);
        }

        EXPECT_EQ(RealSubscriptionExample::REAL_SORTED_LIST, subject_register.NextSubscriptionSync()->GetSubjects());
    }
}

TEST(SubjectSetRegisterTest, test_registering_some_subjects_results_in_no_controls)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT2, false);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    EXPECT_TRUE(message->GetControls().empty());
}

TEST(SubjectSetRegisterTest, test_duplicate_non_refresh_registrations_do_not_add_controls)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT0, false);

    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT1, false);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    ASSERT_TRUE(message);
    EXPECT_TRUE(message->GetControls().empty());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_refresh_registration_adds_a_refresh_control_to_sync)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    EXPECT_TRUE(message);

    subject_register.Register(SS_SUBJECT1, true);
    message = subject_register.NextSubscriptionSync();

    auto actual_message = message->GetControls().begin();

    EXPECT_EQ(0, message->GetControls().size());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_refresh_registrations_adds_a_refresh_control_to_sync_with_correct_sid)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.Register(SS_SUBJECT3, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();

    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT2, true);
    message = subject_register.NextSubscriptionSync();

    SubscriptionSync* actual_message = message.operator->();

    EXPECT_EQ(ControlOperationEnum::REFRESH, message->GetControls().begin()->second);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_refresh_registrations_only_results_in_the_edition_being_unchanged)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();

    subject_register.Register(SS_SUBJECT0, true);
    message = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message->IsChangedEdition());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_refresh_controls_are_not_removed_as_results_of_a_non_refresh_registration)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();

    subject_register.Register(SS_SUBJECT0, true); // causes refresh
    subject_register.Register(SS_SUBJECT0, false); // no refresh required by does no revert the refresh
    message = subject_register.NextSubscriptionSync();
    EXPECT_EQ(ControlOperationEnum::REFRESH, message->GetControls().begin()->second);
}

TEST(SubjectSetRegisterTest, test_refreshing_subjects_which_are_not_already_registered_no_controls_and_new_edition)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT0, true);
    subject_register.Register(SS_SUBJECT1, true);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    EXPECT_TRUE(message->GetControls().empty());

    subject_register.Register(SS_SUBJECT2, true);
    message = subject_register.NextSubscriptionSync();
    EXPECT_TRUE(message->GetControls().empty());
}

TEST(SubjectSetRegisterTest, test_retrieving_after_no_change_returns_null)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    EXPECT_FALSE(subject_register.NextSubscriptionSync());

    subject_register.Register(SS_SUBJECT0, false);
    subject_register.NextSubscriptionSync();
    EXPECT_FALSE(subject_register.NextSubscriptionSync());

    subject_register.Register(SS_SUBJECT0, false);
    EXPECT_FALSE(subject_register.NextSubscriptionSync());
}

TEST(SubjectSetRegisterTest, test_first_subscribe_then_unsub_then_subscribe_conflated_into_one_subscription_with_no_controls)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Unregister(SS_SUBJECT0);
    subject_register.Register(SS_SUBJECT0, false);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    EXPECT_TRUE(message->GetControls().empty());
    EXPECT_EQ(1, message->GetSubjects().size());
    EXPECT_EQ(SS_SUBJECT0, message->GetSubjects()[0]);
}

TEST(SubjectSetRegisterTest, test_sub_unsub_and_sub_with_refresh_conflated_into_one_sub_with_no_controls)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Unregister(SS_SUBJECT0);
    subject_register.Register(SS_SUBJECT0, true);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    EXPECT_TRUE(message->GetControls().empty());
    std::vector<Subject> set_comparison = {SS_SUBJECT0};
    EXPECT_EQ(set_comparison, message->GetSubjects());
}

TEST(SubjectSetRegisterTest, test_RFS_subject_is_configured_right)
{
    EXPECT_EQ("Stream", SS_RFS_SUBJECT1.GetComponent("RequestFor"));
}

TEST(SubjectSetRegisterTest, test_RFQ_subject_is_configured_right)
{
    EXPECT_EQ("Quote", SS_RFQ_SUBJECT3.GetComponent("RequestFor"));
}

TEST(SubjectSetRegisterTest, test_unsubbing_and_subscribing_again_to_an_RFS_result_in_not_subscription_sync_required)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_RFS_SUBJECT1, false);
    subject_register.NextSubscriptionSync();

    subject_register.Unregister(SS_RFS_SUBJECT1);
    subject_register.Register(SS_RFS_SUBJECT1, false);
    EXPECT_FALSE(subject_register.NextSubscriptionSync());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_RFQ_toggle_only_adds_unsubscribe_and_refresh_control)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    subject_register.NextSubscriptionSync();

    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    EXPECT_EQ(ControlOperationEnum::TOGGLE, subject_register.NextSubscriptionSync()->GetControls().begin()->second);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_RFQ_toggle_only_adds_unsubscribe_and_refresh_control_even_if_refreshed)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    subject_register.NextSubscriptionSync();

    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    subject_register.Register(SS_RFQ_SUBJECT3, true); // extra subscription refresh attempt
    EXPECT_EQ(ControlOperationEnum::TOGGLE, subject_register.NextSubscriptionSync()->GetControls().begin()->second);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_RFQ_toggle_only_adds_unsubscribe_and_refresh_control_with_correct_SID)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    subject_register.Register(SS_SUBJECT3, false);
    subject_register.NextSubscriptionSync();

    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();

    EXPECT_EQ(ControlOperationEnum::TOGGLE, message->GetControls().begin()->second);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_RFQ_toggle_only_results_in_the_edition_being_unchanged)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    subject_register.NextSubscriptionSync();

    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    EXPECT_FALSE(subject_register.NextSubscriptionSync()->IsChangedEdition());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_RFQ_toggle_only_results_in_the_edition_number_being_reused)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    std::optional<SubscriptionSync> message1 = subject_register.NextSubscriptionSync();

    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    std::optional<SubscriptionSync> message2 = subject_register.NextSubscriptionSync();
    EXPECT_EQ(message1->GetEdition(), message2->GetEdition());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_unsub_and_sub_multiple_instruments_controls_are_in_correct_order)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    subject_register.Register(SS_RFS_SUBJECT1, false);
    std::vector<Subject> subjects_set = {SS_SUBJECT0, SS_RFS_SUBJECT1, SS_SUBJECT2, SS_SUBJECT3};
    std::optional<SubscriptionSync> message1 = subject_register.NextSubscriptionSync();
    EXPECT_EQ(subjects_set, message1->GetSubjects());

    subject_register.Unregister(SS_SUBJECT0);
    subject_register.Unregister(SS_RFS_SUBJECT1);
    subject_register.Unregister(SS_SUBJECT2);
    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_RFS_SUBJECT1, true);
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.Register(SS_RFQ_SUBJECT3, false);

    std::optional<SubscriptionSync> message2 = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message2->IsChangedEdition());
    std::map<uint32_t, ControlOperationEnum> control_set = {{1, ControlOperationEnum ::REFRESH}, {3, ControlOperationEnum::TOGGLE}};
    EXPECT_EQ(control_set, message2->GetControls());
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_unsub_and_sub_multiples_correct_indices_used_in_controls)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    subject_register.Register(SS_RFS_SUBJECT1, false);
    std::vector<Subject> subjects_set = {SS_SUBJECT0, SS_RFS_SUBJECT1, SS_SUBJECT2, SS_RFQ_SUBJECT3};
    std::optional<SubscriptionSync> message1 = subject_register.NextSubscriptionSync();
    EXPECT_EQ(subjects_set, message1->GetSubjects());

    subject_register.Unregister(SS_SUBJECT0);
    subject_register.Unregister(SS_RFS_SUBJECT1);
    subject_register.Unregister(SS_SUBJECT2);
    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_SUBJECT0, false); // revert to subscribed
    subject_register.Register(SS_RFS_SUBJECT1, true); // forces refresh
    subject_register.Register(SS_SUBJECT2, false); // revert to subscribed
    subject_register.Register(SS_RFQ_SUBJECT3, false); // forces toggle unsubscribe and subscribe

    auto controls = subject_register.NextSubscriptionSync()->GetControls();
    EXPECT_TRUE(controls.find(0) == controls.end());
    EXPECT_EQ(ControlOperationEnum::REFRESH, controls.find(1)->second);
    EXPECT_TRUE(controls.find(2) == controls.end());
    EXPECT_EQ(ControlOperationEnum::TOGGLE, controls.find(3)->second);
}

TEST(SubjectSetRegisterTest, next_subscription_sync_may_combine_subscription_set_additions_with_controls)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_RFS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    std::vector<Subject> subjects_set1 = {SS_RFS_SUBJECT1, SS_SUBJECT2, SS_RFQ_SUBJECT3};
    EXPECT_EQ(subjects_set1, subject_register.NextSubscriptionSync()->GetSubjects());

    subject_register.Register(SS_SUBJECT1, true); // forces refresh
    subject_register.Register(SS_SUBJECT0, false); // new addition to subscription set
    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFQ_SUBJECT3, false); // forces toggle unsubscribe and subscribe

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    std::vector<Subject> subjects_set2 = {SS_SUBJECT0, SS_RFS_SUBJECT1, SS_SUBJECT2, SS_RFQ_SUBJECT3};
    EXPECT_TRUE(message->IsChangedEdition());
    EXPECT_EQ(subjects_set2, message->GetSubjects());

    auto controls = message->GetControls();
    EXPECT_EQ(ControlOperationEnum::REFRESH, controls.find(1)->second);
    EXPECT_EQ(ControlOperationEnum::TOGGLE, controls.find(3)->second);

}

TEST(SubjectSetRegisterTest, next_subscription_sync_may_combine_subscription_set_deletions_with_controls)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_RFS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    std::vector<Subject> subjects_set1 = {SS_SUBJECT0, SS_SUBJECT1, SS_SUBJECT2, SS_RFQ_SUBJECT3};
    EXPECT_EQ(subjects_set1, subject_register.NextSubscriptionSync()->GetSubjects());

    subject_register.Register(SS_SUBJECT1, true); // forces refresh
    subject_register.Unregister(SS_SUBJECT0); // new addition to subscription set
    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFQ_SUBJECT3, false); // forces toggle unsubscribe and subscribe

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    std::vector<Subject> subjects_set2 = {SS_RFS_SUBJECT1, SS_SUBJECT2, SS_RFQ_SUBJECT3};
    EXPECT_TRUE(message->IsChangedEdition());
    EXPECT_EQ(subjects_set2, message->GetSubjects());

    auto controls = message->GetControls();
    EXPECT_EQ(ControlOperationEnum::REFRESH, controls.find(0)->second);

    EXPECT_EQ(subject_register.GetSubjectState().end(), subject_register.GetSubjectState().find(SS_SUBJECT0));
}

TEST(SubjectSetRegisterTest, next_subscription_sync_after_next_call_previous_controls_are_cleared_out)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_RFS_SUBJECT1, false);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    subject_register.NextSubscriptionSync()->GetSubjects();

    subject_register.Unregister(SS_RFS_SUBJECT1);
    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFS_SUBJECT1, true);
    subject_register.Register(SS_RFQ_SUBJECT3, false);

    EXPECT_EQ(2, subject_register.NextSubscriptionSync()->GetControls().size());

    subject_register.Register(SS_SUBJECT0, false);
    EXPECT_TRUE(subject_register.NextSubscriptionSync()->GetControls().empty());
}

TEST(SubjectSetRegisterTest, test_spurious_unsub_does_not_affect_subscriptions)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Unregister(SS_SUBJECT0);
    ASSERT_FALSE(subject_register.NextSubscriptionSync());

    subject_register.Unregister(SS_SUBJECT0);
    ASSERT_FALSE(subject_register.NextSubscriptionSync());

    subject_register.Unregister(SS_SUBJECT0);
    ASSERT_FALSE(subject_register.NextSubscriptionSync());

    subject_register.Unregister(SS_SUBJECT1);
    ASSERT_FALSE(subject_register.NextSubscriptionSync());
}

TEST(SubjectSetRegisterTest, test_unsub_and_sub_supercedes_refresh)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    std::vector<Subject> subjects_set = {SS_RFQ_SUBJECT3};
    EXPECT_EQ(subjects_set, subject_register.NextSubscriptionSync()->GetSubjects());

    subject_register.Register(SS_RFQ_SUBJECT3, true);
    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFQ_SUBJECT3, false);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message->IsChangedEdition());

    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFQ_SUBJECT3, true);

    message = subject_register.NextSubscriptionSync();
    EXPECT_FALSE(message->IsChangedEdition());
    std::map<uint32_t, ControlOperationEnum> control_set = {{0, ControlOperationEnum::TOGGLE}};
    EXPECT_EQ(control_set, message->GetControls());
}

TEST(SubjectSetRegisterTest, test_refreshing_then_unsubscribing_no_controls_and_one_less_subject)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_SUBJECT0, false);
    subject_register.Register(SS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.NextSubscriptionSync();

    subject_register.Register(SS_SUBJECT0, true);
    subject_register.Unregister(SS_SUBJECT0);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    std::vector<Subject> subjects_set = {SS_SUBJECT1, SS_SUBJECT2};
    EXPECT_EQ(subjects_set, message->GetSubjects());
    EXPECT_TRUE(message->GetControls().empty());
}

TEST(SubjectSetRegisterTest, test_lots_of_unsubbing_and_subscribing_all_conflated_into_one_unsub_and_sub)
{
    SubjectSetRegister subject_register = SubjectSetRegister();
    subject_register.Register(SS_RFS_SUBJECT1, false);
    subject_register.Register(SS_SUBJECT2, false);
    subject_register.Register(SS_RFQ_SUBJECT3, false);
    subject_register.NextSubscriptionSync();

    subject_register.Register(SS_RFS_SUBJECT1, true);
    subject_register.Unregister(SS_RFS_SUBJECT1);
    subject_register.Register(SS_SUBJECT1, true);
    subject_register.Unregister(SS_SUBJECT1);
    subject_register.Register(SS_SUBJECT1, true);

    subject_register.Register(SS_RFQ_SUBJECT3, true);
    subject_register.Unregister(SS_RFQ_SUBJECT3);
    subject_register.Register(SS_RFQ_SUBJECT3, true);

    std::optional<SubscriptionSync> message = subject_register.NextSubscriptionSync();
    std::vector<Subject> subjects_set = {SS_RFS_SUBJECT1, SS_SUBJECT2, SS_RFQ_SUBJECT3};
    EXPECT_EQ(subjects_set, message->GetSubjects());
    std::map<uint32_t, ControlOperationEnum> control_set = {{0, ControlOperationEnum::REFRESH}, {2, ControlOperationEnum ::TOGGLE}};
    EXPECT_EQ(control_set, message->GetControls());
}

TEST(SubjectSetRegisterTest, test_is_quote)
{
    SubjectSetRegister subject_register = SubjectSetRegister();

    const Subject quote_subject = SubjectUtils::CreateSubjectFromString("RequestFor=Quote");
    EXPECT_TRUE(subject_register.IsRequestForQuote(quote_subject));

    const Subject stream_subject = SubjectUtils::CreateSubjectFromString("RequestFor=Stream");
    EXPECT_TRUE(subject_register.IsRequestForQuote(quote_subject));
}

}