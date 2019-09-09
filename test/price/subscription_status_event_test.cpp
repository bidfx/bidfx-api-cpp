/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include "include/subscription_status_event.h"
#include "include/price/subject/subject.h"
#include "include/subscription_status.h"
#include "include/price/subject/subject_factory.h"
#include "include/price/subject/subject_builder.h"
#include "gtest/gtest.h"

namespace bidfx_public_api
{

using bidfx_public_api::price::subject::SubjectFormatter;
using bidfx_public_api::price::subject::SubjectBuilder;

class SubscriptionStatusEventTest : public testing::Test
{
public:
    Subject subject;
    SubscriptionStatus status;
    std::string explanation;
    std::unique_ptr<SubscriptionStatusEvent> event;

    SubscriptionStatusEventTest()
    {
        SubjectBuilder builder;
        SubjectFormatter().ParseSubject("BuySideAccount=FX_ACCT,Currency=EUR,DealType=Spot,Quantity=100000,Symbol=EURUSD", builder);
        subject = builder.CreateSubject();
        status = SubscriptionStatus::REJECTED;
        explanation = "Dealer Rejected";
        event = std::make_unique<SubscriptionStatusEvent>(subject, status, explanation);
    }
};

TEST_F(SubscriptionStatusEventTest, TestGetSubject)
{
    EXPECT_EQ(subject, event->GetSubject());
}

TEST_F(SubscriptionStatusEventTest, TestGetStatus)
{
    EXPECT_EQ(status, event->GetStatus());
}

TEST_F(SubscriptionStatusEventTest, TestGetExplanation)
{
    EXPECT_EQ(explanation, event->GetExplanation());
}

}