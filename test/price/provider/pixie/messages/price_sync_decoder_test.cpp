/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include <deque>
#include <memory>
#include "src/price/provider/pixie/messages/syncable.h"
#include "src/price/provider/pixie/fields/extendable_data_dictionary.h"
#include "src/price/provider/pixie/messages/price_event.h"
#include "src/price/provider/pixie/messages/price_sync_decoder.h"
#include "src/tools/buffer_util.h"
#include "gtest/gtest.h"
#include "pixie_test_helper.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::BufferUtil;


class PriceSyncDecoderTest: public ::testing::Test
{
public:
    PriceSyncDecoder decoder;
    ExtendableDataDictionary data_dictionary = ExtendableDataDictionary();

    PriceSyncDecoderTest()
    {
        data_dictionary.AddFieldDef(FieldDef(0, FieldTypeEnum::DOUBLE, "Bid"));
        data_dictionary.AddFieldDef(FieldDef(1, FieldTypeEnum::LONG, "BidSize"));
        data_dictionary.AddFieldDef(FieldDef(2, FieldTypeEnum::DOUBLE, "Ask"));
        data_dictionary.AddFieldDef(FieldDef(3, FieldTypeEnum::LONG, "AskSize"));
    }

    void ExpectPriceImageEvent(PriceEvent& event);
    void ExpectPriceUpdateEvent(PriceEvent &event);

    void ExpectPriceContains(std::map<std::string, PriceField>& fields, PriceEvent& event);
};

void PriceSyncDecoderTest::ExpectPriceImageEvent(PriceEvent& event)
{
    EXPECT_EQ("PriceImageEvent", event.GetType());
}

void PriceSyncDecoderTest::ExpectPriceUpdateEvent(PriceEvent& event)
{
    EXPECT_EQ("PriceUpdateEvent", event.GetType());
}

void PriceSyncDecoderTest::ExpectPriceContains(std::map<std::string, PriceField>& fields, PriceEvent& event)
{
    std::map<std::string, PriceField> price_map = event.GetPrice();
    for (auto const& entry : fields)
    {
        std::map<std::string, PriceField>::iterator element = price_map.find(entry.first);
        EXPECT_NE(price_map.end(), element);
        if (element != price_map.end())
        {
            EXPECT_EQ(entry.second, element->second);
        }
    }
}

TEST_F(PriceSyncDecoderTest, TestDecodingAnUncompressedMessage)
{
    ByteBuffer buffer = BufferUtil::HexBuffer("500007b6b0b1e4f12c2f0403660004003ff8000000000000010000000000000bb8023ff8ac083126e979030000000000000bb8660104003ff5b645a1cac0830100000000000007d0023ff5d70a3d70a3d70300000000000007d06602040040015604189374bc0100000000000011940240017ae147ae147b030000000000001194");
    EXPECT_EQ('P', buffer.ReadByte());
    PriceSync price_sync = decoder.DecodePriceSync(buffer);
    EXPECT_EQ(7, price_sync.GetRevision());
    EXPECT_EQ(1542372218934, price_sync.GetRevisionTime());
    EXPECT_EQ(47, price_sync.GetConflationLatency());
    EXPECT_EQ(4, price_sync.GetEdition());
    EXPECT_EQ(3, price_sync.GetSize());
    EXPECT_FALSE(price_sync.IsCompressed());

    std::deque<std::shared_ptr<PriceEvent>> events;
    auto syncable = QueueingPriceSyncListener(&events);
    price_sync.Visit(data_dictionary, syncable);

    ASSERT_FALSE(events.empty());

    std::shared_ptr<PriceEvent> event = events.front();
    events.pop_front();
    std::map<std::string, PriceField> expected1;
    expected1.emplace("Ask", PriceField(1.542));
    expected1.emplace("AskSize", PriceField(3000));
    expected1.emplace("Bid", PriceField(1.5));
    expected1.emplace("BidSize", PriceField(3000));
    EXPECT_EQ(0, event->GetSid());
    ExpectPriceImageEvent(*event);
    ExpectPriceContains(expected1, *event);

    event = events.front();
    events.pop_front();
    std::map<std::string, PriceField> expected2;
    expected2.emplace("Ask", PriceField(1.365));
    expected2.emplace("AskSize", PriceField(2000));
    expected2.emplace("Bid", PriceField(1.357));
    expected2.emplace("BidSize", PriceField(2000));
    EXPECT_EQ(1, event->GetSid());
    ExpectPriceImageEvent(*event);
    ExpectPriceContains(expected2, *event);

    event = events.front();
    events.pop_front();
    std::map<std::string, PriceField> expected3;
    expected3.emplace("Ask", PriceField(2.185));
    expected3.emplace("AskSize", PriceField(4500));
    expected3.emplace("Bid", PriceField(2.167));
    expected3.emplace("BidSize", PriceField(4500));
    EXPECT_EQ(2, event->GetSid());
    ExpectPriceImageEvent(*event);
    ExpectPriceContains(expected3, *event);

    EXPECT_TRUE(events.empty());
}

TEST_F(PriceSyncDecoderTest, TestDecodingACompressedMessage)
{
    ByteBuffer buffer = BufferUtil::HexBuffer("500103858afa8ee82c6337034a636061b0ffc100068c108a7b0793fd8f351c866a2f2b996122698c40655fb7b92e3c75a019aa8cfd0293fdd7eb5cb6058baf33c344d29858181c18c358242697ec812a139cc2e4c058f5d07d9d4835334c04000000ffff");
    EXPECT_EQ('P', buffer.ReadByte());
    PriceSync price_sync = decoder.DecodePriceSync(buffer);
    EXPECT_EQ(3, price_sync.GetRevision());
    EXPECT_EQ(1539777135877, price_sync.GetRevisionTime());
    EXPECT_EQ(99, price_sync.GetConflationLatency());
    EXPECT_EQ(55, price_sync.GetEdition());
    EXPECT_EQ(3, price_sync.GetSize());
    EXPECT_TRUE(price_sync.IsCompressed());

    std::deque<std::shared_ptr<PriceEvent>> events;
    auto syncable = QueueingPriceSyncListener(&events);
    price_sync.Visit(data_dictionary, syncable);

    ASSERT_FALSE(events.empty());

    std::shared_ptr<PriceEvent> event = events.front();
    events.pop_front();
    std::map<std::string, PriceField> expected1;
    expected1.emplace("Ask", PriceField(1.542));
    expected1.emplace("AskSize", PriceField(3000));
    expected1.emplace("Bid", PriceField(1.5));
    expected1.emplace("BidSize", PriceField(3000));
    EXPECT_EQ(0, event->GetSid());
    ExpectPriceImageEvent(*event);
    ExpectPriceContains(expected1, *event);

    event = events.front();
    events.pop_front();
    std::map<std::string, PriceField> expected2;
    expected2.emplace("Ask", PriceField(1.365));
    expected2.emplace("AskSize", PriceField(2000));
    expected2.emplace("Bid", PriceField(1.357));
    expected2.emplace("BidSize", PriceField(2000));
    EXPECT_EQ(1, event->GetSid());
    ExpectPriceImageEvent(*event);
    ExpectPriceContains(expected2, *event);

    event = events.front();
    events.pop_front();
    std::map<std::string, PriceField> expected3;
    expected3.emplace("Ask", PriceField(2.185));
    expected3.emplace("AskSize", PriceField(4500));
    expected3.emplace("Bid", PriceField(2.167));
    expected3.emplace("BidSize", PriceField(4500));
    EXPECT_EQ(2, event->GetSid());
    ExpectPriceImageEvent(*event);
    ExpectPriceContains(expected3, *event);

    EXPECT_TRUE(events.empty());
}

TEST_F(PriceSyncDecoderTest, TestDecodingFullPriceUpdate)
{
    ByteBuffer buffer = BufferUtil::HexBuffer("500007b6b0b1e4f12c2f0401" // 'P' - 0 - 7 - 1542372218934 - 47 - 4 - 1
                                                               "66" // 'f' - type
                                                               "08" // 8 - SID
                                                               "04" // 4 - field count
                                                                   "00" // 0 - FID (Bid)
                                                                   "4011c5a1cac08312" // 4.443
                                                                   "01" // 1 - FID (BidSize)
                                                                   "000000000000157c" // 5500
                                                                   "02" // 2 - FID (Ask)
                                                                   "401d607dd4413554" // 7.34423
                                                                   "03" // 3 - FID (AskSize)
                                                                   "0000000000001db0" // 7600
                                                                   );
    EXPECT_EQ('P', buffer.ReadByte());
    PriceSync price_sync = decoder.DecodePriceSync(buffer);
    EXPECT_EQ(7, price_sync.GetRevision());
    EXPECT_EQ(1542372218934, price_sync.GetRevisionTime());
    EXPECT_EQ(47, price_sync.GetConflationLatency());
    EXPECT_EQ(4, price_sync.GetEdition());
    EXPECT_EQ(1, price_sync.GetSize());
    EXPECT_FALSE(price_sync.IsCompressed());

    std::deque<std::shared_ptr<PriceEvent>> events;
    auto syncable = QueueingPriceSyncListener(&events);
    price_sync.Visit(data_dictionary, syncable);

    ASSERT_FALSE(events.empty());

    std::shared_ptr<PriceEvent> event = events.front();
    events.pop_front();
    std::map<std::string, PriceField> expected1;
    expected1.emplace("Ask", PriceField(7.34423));
    expected1.emplace("AskSize", PriceField(7600));
    expected1.emplace("Bid", PriceField(4.443));
    expected1.emplace("BidSize", PriceField(5500));
    EXPECT_EQ(8, event->GetSid());
    ExpectPriceImageEvent(*event);
    ExpectPriceContains(expected1, *event);

    EXPECT_TRUE(events.empty());
}

TEST_F(PriceSyncDecoderTest, TestDecodingPartialPriceUpdate)
{
    ByteBuffer buffer = BufferUtil::HexBuffer("500007b6b0b1e4f12c2f0401" // 'P' - 0 - 7 - 1542372218934 - 47 - 4 - 1
                                                               "70" // 'p' - type
                                                               "08" // 8 - SID
                                                               "04" // 4 - field count
                                                               "00" // 0 - FID (Bid)
                                                               "4011c5a1cac08312" // 4.443
                                                               "01" // 1 - FID (BidSize)
                                                               "000000000000157c" // 5500
                                                               "02" // 2 - FID (Ask)
                                                               "401d607dd4413554" // 7.34423
                                                               "03" // 3 - FID (AskSize)
                                                               "0000000000001db0" // 7600
    );
    EXPECT_EQ('P', buffer.ReadByte());
    PriceSync price_sync = decoder.DecodePriceSync(buffer);
    EXPECT_EQ(7, price_sync.GetRevision());
    EXPECT_EQ(1542372218934, price_sync.GetRevisionTime());
    EXPECT_EQ(47, price_sync.GetConflationLatency());
    EXPECT_EQ(4, price_sync.GetEdition());
    EXPECT_EQ(1, price_sync.GetSize());
    EXPECT_FALSE(price_sync.IsCompressed());

    std::deque<std::shared_ptr<PriceEvent>> events;
    auto syncable = QueueingPriceSyncListener(&events);
    price_sync.Visit(data_dictionary, syncable);

    ASSERT_FALSE(events.empty());

    std::shared_ptr<PriceEvent> event = events.front();
    events.pop_front();
    std::map<std::string, PriceField> expected1;
    expected1.emplace("Ask", PriceField(7.34423));
    expected1.emplace("AskSize", PriceField(7600));
    expected1.emplace("Bid", PriceField(4.443));
    expected1.emplace("BidSize", PriceField(5500));
    EXPECT_EQ(8, event->GetSid());
    ExpectPriceUpdateEvent(*event);
    ExpectPriceContains(expected1, *event);

    EXPECT_TRUE(events.empty());
}

}