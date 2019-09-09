/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include <sstream>
#include <string>
#include "pixie_test_helper.h"

namespace bidfx_public_api::price::pixie
{

QueueingPriceSyncListener::QueueingPriceSyncListener(std::deque<std::shared_ptr<PriceEvent>>* queue)
{
    queue_ = queue;
}

void QueueingPriceSyncListener::PriceImage(uint32_t sid, std::map<std::string, PriceField> price)
{
    queue_->emplace_back(std::shared_ptr<PriceEvent>((PriceEvent*) new PriceImageEvent(sid, price)));
}

void QueueingPriceSyncListener::PriceUpdate(uint32_t sid, std::map<std::string, PriceField> price)
{
    queue_->emplace_back(std::shared_ptr<PriceEvent>((PriceEvent*) new PriceUpdateEvent(sid, price)));
}

void QueueingPriceSyncListener::PriceStatus(uint32_t sid, SubscriptionStatus status, std::string explanation)
{}


PriceMapEvent::PriceMapEvent(uint32_t sid, std::map<std::string, PriceField> price)
{
    sid_ = sid;
    price_ = price;
}

std::map<std::string, PriceField> PriceMapEvent::GetPrice()
{
    return price_;
}

std::string PriceMapEvent::GetPriceString()
{
    std::stringstream ss;
    ss << "{";
    for (auto it = price_.cbegin(); it != price_.cend(); it++)
    {
        ss << (it->first) << "=" << (it->second) << ", ";
    }
    ss << "}";
    return ss.str();
}

uint32_t PriceMapEvent::GetSid()
{
    return sid_;
}

QueueingPriceSyncListener::PriceImageEvent::PriceImageEvent(uint32_t sid, std::map<std::string, PriceField> price)
        : PriceMapEvent(sid, price)
{}

std::string QueueingPriceSyncListener::PriceImageEvent::GetType()
{
    return "PriceImageEvent";
}

std::string QueueingPriceSyncListener::PriceImageEvent::ToString()
{
    std::stringstream ss;
    ss << GetType();
    ss << " [sid=";
    ss << GetSid();
    ss << ", price=";
    ss << GetPriceString();
    ss << "]";
    return ss.str();
}

QueueingPriceSyncListener::PriceUpdateEvent::PriceUpdateEvent(uint32_t sid, std::map<std::string, PriceField> price) :
    PriceMapEvent(sid, price)
{}

std::string QueueingPriceSyncListener::PriceUpdateEvent::GetType()
{
    return "PriceUpdateEvent";
}

std::string QueueingPriceSyncListener::PriceUpdateEvent::ToString()
{
    std::stringstream ss;
    ss << GetType();
    ss << " [sid=";
    ss << GetSid();
    ss << ", price=";
    ss << GetPriceString();
    ss << "]";
    return ss.str();
}

}//namespace