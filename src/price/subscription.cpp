/**
 * Copyright 2019 BidFX Systems Ltd. All rights reserved.
 */

#include "subscription.h"

namespace bidfx_public_api::price
{

Subscription::Subscription(Subject subject) : subject_(subject), status_(SubscriptionStatus::PENDING)
{}

Subscription::~Subscription()
{}

Subscription::Subscription(const bidfx_public_api::price::Subscription& to_copy)
{
    status_ = to_copy.status_;
    price_fields_ = to_copy.price_fields_;
    subject_ = to_copy.subject_;
}

Subscription::Subscription(bidfx_public_api::price::Subscription&& to_move)
{
    status_ = std::move(to_move.status_);
    price_fields_ = std::move(to_move.price_fields_);
    subject_ = std::move(to_move.subject_);
}

Subscription& Subscription::operator=(const bidfx_public_api::price::Subscription& to_copy)
{
    status_ = to_copy.status_;
    price_fields_ = to_copy.price_fields_;
    subject_ = to_copy.subject_;
    return *this;
}

Subscription& Subscription::operator=(bidfx_public_api::price::Subscription&& to_move)
{
    status_ = std::move(to_move.status_);
    price_fields_ = std::move(to_move.price_fields_);
    subject_ = std::move(to_move.subject_);
    return *this;
}

const SubscriptionStatus& Subscription::GetStatus() const
{
    return status_;
}

void Subscription::SetStatus(const SubscriptionStatus status)
{
    status_ = status;
}

const std::map<std::string, PriceField>& Subscription::GetAllPriceFields() const
{
    return price_fields_;
}

const Subject& Subscription::GetSubject() const
{
    return subject_;
}

void Subscription::MergePriceMap(const std::map<std::string, PriceField>& price_update,
                                 bool replace_all_fields)
{
    auto bid_levels_it = price_fields_.find("BidLevels");
    auto ask_levels_it = price_fields_.find("AskLevels");
    int32_t oldBidLevels = bid_levels_it != price_fields_.end() ? bid_levels_it->second.GetInt() : 0;
    int32_t oldAskLevels = ask_levels_it != price_fields_.end() ? ask_levels_it->second.GetInt() : 0;

    if (replace_all_fields)
    {
        price_fields_.clear();
    }

    for (const auto field : price_update)
    {
        price_fields_[field.first] = field.second;
    }

    bid_levels_it = price_fields_.find("BidLevels");
    ask_levels_it = price_fields_.find("AskLevels");
    int32_t new_bid_levels = bid_levels_it != price_fields_.end() ? bid_levels_it->second.GetInt() : 0;
    int32_t new_ask_levels = ask_levels_it != price_fields_.end() ? ask_levels_it->second.GetInt() : 0;

    for (int32_t i = new_bid_levels + 1; i <= oldBidLevels; i++)
    {
        price_fields_.erase("Bid" + std::to_string(i));
        price_fields_.erase("BidSize" + std::to_string(i));
        price_fields_.erase("BidFirm" + std::to_string(i));
        price_fields_.erase("BidTime" + std::to_string(i));
        price_fields_.erase("BidID" + std::to_string(i));
        price_fields_.erase("BidSpot" + std::to_string(i));
        price_fields_.erase("BidForwardPoints" + std::to_string(i));
    }
    for (int32_t i = new_ask_levels + 1; i <= oldAskLevels; i++)
    {
        price_fields_.erase("Ask" + std::to_string(i));
        price_fields_.erase("AskSize" + std::to_string(i));
        price_fields_.erase("AskFirm" + std::to_string(i));
        price_fields_.erase("AskTime" + std::to_string(i));
        price_fields_.erase("AskID" + std::to_string(i));
        price_fields_.erase("AskSpot" + std::to_string(i));
        price_fields_.erase("AskForwardPoints" + std::to_string(i));
    }
}

void Subscription::ClearPriceFields()
{
    price_fields_.clear();
}

}