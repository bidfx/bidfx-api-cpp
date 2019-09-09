/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_PIXIE_PRICE_MESSAGES_PIXIE_TEST_HELPER_H_
#define PUBLIC_API_CPP_PIXIE_PRICE_MESSAGES_PIXIE_TEST_HELPER_H_

#include <deque>
#include <memory>
#include "src/price/provider/pixie/messages/syncable.h"
#include "src/price/provider/pixie/messages/price_event.h"

namespace bidfx_public_api::price::pixie
{
class PriceMapEvent : PriceEvent
{
private:
    uint32_t sid_;
    std::map<std::string, PriceField> price_;
public:
    PriceMapEvent(uint32_t sid, std::map<std::string, PriceField> price);
    uint32_t GetSid();
    std::map<std::string, PriceField> GetPrice();
    std::string GetPriceString();
};

class QueueingPriceSyncListener : public Syncable
{
private:
    std::deque<std::shared_ptr<PriceEvent>> *queue_;
public:
    QueueingPriceSyncListener(std::deque<std::shared_ptr<PriceEvent>>* queue);

    void PriceImage(uint32_t sid, std::map<std::string, PriceField> price) override;

private:
    void PriceUpdate(uint32_t sid, std::map<std::string, PriceField> price) override;

    void PriceStatus(uint32_t sid, SubscriptionStatus status, std::string explanation) override;

    class PriceImageEvent : public PriceMapEvent
    {
    public:
        PriceImageEvent(uint32_t sid, std::map<std::string, PriceField> price);
        std::string ToString() override;
        std::string GetType() override;
    };

    class PriceUpdateEvent : public PriceMapEvent
    {
    public:
        PriceUpdateEvent(uint32_t sid, std::map<std::string, PriceField> price);
        std::string ToString() override;
        std::string GetType() override;
    };
};
}

#endif //PUBLIC_API_CPP_PIXIE_PRICE_MESSAGES_PIXIE_TEST_HELPER_H_
