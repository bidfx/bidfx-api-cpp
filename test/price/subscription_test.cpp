/**
 * Copyright 2019 BidFX Systems Ltd. All rights reserved.
 */

#include <gtest/gtest.h>
#include "include/price/subject/subject_factory.h"
#include "src/basic_user_info.h"
#include "src/price/subscription.h"

namespace bidfx_public_api::price
{

class SubscriptionTest: public ::testing::Test
{
public:
    UserInfo* user_info;
    Subject subject_;
    Subscription* subscription_;

    SubscriptionTest()
    {
        user_info = new BasicUserInfo("localhost", "lasman", "FX_ACCT", "CPP", "0", "-");
        subject_ = subject::SubjectFactory(user_info).Fx().Stream().Forward().BuySideAccount("FX_ACCT").Currency("EUR").CurrencyPair("EURUSD").Quantity(1000).Tenor(Tenor::IN_2_MONTHS).LiquidityProvider("DBFX").CreateSubject();
        subscription_ = new Subscription(subject_);
    }

    ~SubscriptionTest()
    {
        delete user_info;
        delete subscription_;
    }
};

TEST_F(SubscriptionTest, TestMergingPriceMapWithFewerBidLevelsClearsExcessLevels)
{
    std::map<std::string, PriceField> price_map;
    price_map["BidLevels"] = PriceField(5);
    price_map["AskLevels"] = PriceField(5);
    price_map["BidFirm1"] = PriceField("Firm1");
    price_map["BidFirm2"] = PriceField("Firm2");
    price_map["BidFirm3"] = PriceField("Firm3");
    price_map["BidFirm4"] = PriceField("Firm4");
    price_map["BidFirm5"] = PriceField("Firm5");
    price_map["AskFirm1"] = PriceField("Firm1");
    price_map["AskFirm2"] = PriceField("Firm2");
    price_map["AskFirm3"] = PriceField("Firm3");
    price_map["AskFirm4"] = PriceField("Firm4");
    price_map["AskFirm5"] = PriceField("Firm5");
    price_map["BidSize1"] = PriceField(static_cast<int64_t>(1000L));
    price_map["BidSize2"] = PriceField(static_cast<int64_t>(1000L));
    price_map["BidSize3"] = PriceField(static_cast<int64_t>(1000L));
    price_map["BidSize4"] = PriceField(static_cast<int64_t>(1000L));
    price_map["BidSize5"] = PriceField(static_cast<int64_t>(1000L));
    price_map["AskSize1"] = PriceField(static_cast<int64_t>(1000L));
    price_map["AskSize2"] = PriceField(static_cast<int64_t>(1000L));
    price_map["AskSize3"] = PriceField(static_cast<int64_t>(1000L));
    price_map["AskSize4"] = PriceField(static_cast<int64_t>(1000L));
    price_map["AskSize5"] = PriceField(static_cast<int64_t>(1000L));
    price_map["Bid1"] = PriceField(1.2304);
    price_map["Bid2"] = PriceField(1.2303);
    price_map["Bid3"] = PriceField(1.2302);
    price_map["Bid4"] = PriceField(1.2301);
    price_map["Bid5"] = PriceField(1.2300);
    price_map["Ask1"] = PriceField(1.2305);
    price_map["Ask2"] = PriceField(1.2306);
    price_map["Ask3"] = PriceField(1.2307);
    price_map["Ask4"] = PriceField(1.2308);
    price_map["Ask5"] = PriceField(1.2309);
    price_map["BidTime1"] = PriceField(static_cast<int64_t>(2500L));
    price_map["BidTime2"] = PriceField(static_cast<int64_t>(2501L));
    price_map["BidTime3"] = PriceField(static_cast<int64_t>(2502L));
    price_map["BidTime4"] = PriceField(static_cast<int64_t>(2503L));
    price_map["BidTime5"] = PriceField(static_cast<int64_t>(2504L));
    price_map["AskTime1"] = PriceField(static_cast<int64_t>(1500L));
    price_map["AskTime2"] = PriceField(static_cast<int64_t>(1501L));
    price_map["AskTime3"] = PriceField(static_cast<int64_t>(1502L));
    price_map["AskTime4"] = PriceField(static_cast<int64_t>(1503L));
    price_map["AskTime5"] = PriceField(static_cast<int64_t>(1504L));
    price_map["BidID1"] = PriceField("ID-1");
    price_map["BidID2"] = PriceField("ID-2");
    price_map["BidID3"] = PriceField("ID-3");
    price_map["BidID4"] = PriceField("ID-4");
    price_map["BidID5"] = PriceField("ID-5");
    price_map["AskID1"] = PriceField("ID-1");
    price_map["AskID2"] = PriceField("ID-2");
    price_map["AskID3"] = PriceField("ID-3");
    price_map["AskID4"] = PriceField("ID-4");
    price_map["AskID5"] = PriceField("ID-5");
    price_map["BidSpot1"] = PriceField(1.0001);
    price_map["BidSpot2"] = PriceField(1.0002);
    price_map["BidSpot3"] = PriceField(1.0003);
    price_map["BidSpot4"] = PriceField(1.0004);
    price_map["BidSpot5"] = PriceField(1.0005);
    price_map["AskSpot1"] = PriceField(1.0006);
    price_map["AskSpot2"] = PriceField(1.0007);
    price_map["AskSpot3"] = PriceField(1.0008);
    price_map["AskSpot4"] = PriceField(1.0009);
    price_map["AskSpot5"] = PriceField(1.0010);
    price_map["BidForwardPoints1"] = PriceField(0.0012);
    price_map["BidForwardPoints2"] = PriceField(0.0022);
    price_map["BidForwardPoints3"] = PriceField(0.0032);
    price_map["BidForwardPoints4"] = PriceField(0.0042);
    price_map["BidForwardPoints5"] = PriceField(0.0052);
    price_map["AskForwardPoints1"] = PriceField(0.0001);
    price_map["AskForwardPoints2"] = PriceField(0.0002);
    price_map["AskForwardPoints3"] = PriceField(0.0003);
    price_map["AskForwardPoints4"] = PriceField(0.0004);
    price_map["AskForwardPoints5"] = PriceField(0.0005);
    subscription_->MergePriceMap(price_map, false);

    std::map<std::string, PriceField> new_price_map;
    new_price_map["BidLevels"] = PriceField(2);
    new_price_map["AskLevels"] = PriceField(3);
    new_price_map["BidFirm2"] = PriceField("NewFirm2");
    new_price_map["AskFirm1"] = PriceField("NewFirm1");
    new_price_map["AskFirm2"] = PriceField("Firm1");

    subscription_->MergePriceMap(new_price_map, false);

    std::map<std::string, PriceField> result = subscription_->GetAllPriceFields();
    EXPECT_EQ(3, result["AskLevels"].GetInt());
    EXPECT_EQ(2, result["BidLevels"].GetInt());
    EXPECT_EQ("NewFirm1", result["AskFirm1"].GetString());
    EXPECT_EQ("Firm1", result["AskFirm2"].GetString());
    EXPECT_EQ("Firm3", result["AskFirm3"].GetString());
    EXPECT_EQ(result.end(), result.find("AskFirm4"));
    EXPECT_EQ(result.end(), result.find("AskFirm5"));
    EXPECT_EQ("Firm1", result["BidFirm1"].GetString());
    EXPECT_EQ("NewFirm2", result["BidFirm2"].GetString());
    EXPECT_EQ(result.end(), result.find("BidFirm3"));
    EXPECT_EQ(result.end(), result.find("BidFirm4"));
    EXPECT_EQ(result.end(), result.find("BidFirm5"));
    EXPECT_EQ(1000, result["BidSize1"].GetLong());
    EXPECT_EQ(1000, result["BidSize2"].GetLong());
    EXPECT_EQ(result.end(), result.find("BidSize3"));
    EXPECT_EQ(result.end(), result.find("BidSize4"));
    EXPECT_EQ(result.end(), result.find("BidSize5"));
    EXPECT_EQ(1000, result["AskSize1"].GetLong());
    EXPECT_EQ(1000, result["AskSize2"].GetLong());
    EXPECT_EQ(1000, result["AskSize3"].GetLong());
    EXPECT_EQ(result.end(), result.find("AskSize4"));
    EXPECT_EQ(result.end(), result.find("AskSize5"));
    EXPECT_EQ(1.2304, result["Bid1"].GetDouble());
    EXPECT_EQ(1.2303, result["Bid2"].GetDouble());
    EXPECT_EQ(result.end(), result.find("Bid3"));
    EXPECT_EQ(result.end(), result.find("Bid4"));
    EXPECT_EQ(result.end(), result.find("Bid5"));
    EXPECT_EQ(1.2305, result["Ask1"].GetDouble());
    EXPECT_EQ(1.2306, result["Ask2"].GetDouble());
    EXPECT_EQ(1.2307, result["Ask3"].GetDouble());
    EXPECT_EQ(result.end(), result.find("Ask4"));
    EXPECT_EQ(result.end(), result.find("Ask5"));
    EXPECT_EQ(2500, result["BidTime1"].GetLong());
    EXPECT_EQ(2501, result["BidTime2"].GetLong());
    EXPECT_EQ(result.end(), result.find("BidTime3"));
    EXPECT_EQ(result.end(), result.find("BidTime4"));
    EXPECT_EQ(result.end(), result.find("BidTime5"));
    EXPECT_EQ(1500, result["AskTime1"].GetLong());
    EXPECT_EQ(1501, result["AskTime2"].GetLong());
    EXPECT_EQ(1502, result["AskTime3"].GetLong());
    EXPECT_EQ(result.end(), result.find("AskTime4"));
    EXPECT_EQ(result.end(), result.find("AskTime5"));
    EXPECT_EQ("ID-1", result["BidID1"].GetString());
    EXPECT_EQ("ID-2", result["BidID2"].GetString());
    EXPECT_EQ(result.end(), result.find("BidID3"));
    EXPECT_EQ(result.end(), result.find("BidID4"));
    EXPECT_EQ(result.end(), result.find("BidID5"));
    EXPECT_EQ("ID-1", result["AskID1"].GetString());
    EXPECT_EQ("ID-2", result["AskID2"].GetString());
    EXPECT_EQ("ID-3", result["AskID3"].GetString());
    EXPECT_EQ(result.end(), result.find("AskID4"));
    EXPECT_EQ(result.end(), result.find("AskID5"));
    EXPECT_EQ(1.0001, result["BidSpot1"].GetDouble());
    EXPECT_EQ(1.0002, result["BidSpot2"].GetDouble());
    EXPECT_EQ(result.end(), result.find("BidSpot3"));
    EXPECT_EQ(result.end(), result.find("BidSpot4"));
    EXPECT_EQ(result.end(), result.find("BidSpot5"));
    EXPECT_EQ(1.0006, result["AskSpot1"].GetDouble());
    EXPECT_EQ(1.0007, result["AskSpot2"].GetDouble());
    EXPECT_EQ(1.0008, result["AskSpot3"].GetDouble());
    EXPECT_EQ(result.end(), result.find("AskSpot4"));
    EXPECT_EQ(result.end(), result.find("AskSpot5"));
    EXPECT_EQ(0.0012, result["BidForwardPoints1"].GetDouble());
    EXPECT_EQ(0.0022, result["BidForwardPoints2"].GetDouble());
    EXPECT_EQ(result.end(), result.find("BidForwardPoints3"));
    EXPECT_EQ(result.end(), result.find("BidForwardPoints4"));
    EXPECT_EQ(result.end(), result.find("BidForwardPoints5"));
    EXPECT_EQ(0.0001, result["AskForwardPoints1"].GetDouble());
    EXPECT_EQ(0.0002, result["AskForwardPoints2"].GetDouble());
    EXPECT_EQ(0.0003, result["AskForwardPoints3"].GetDouble());
    EXPECT_EQ(result.end(), result.find("AskForwardPoints4"));
    EXPECT_EQ(result.end(), result.find("AskForwardPoints5"));
}
}