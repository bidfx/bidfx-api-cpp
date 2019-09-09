/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_SYNCABLE_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_SYNCABLE_H_

#include <string>
#include <vector>
#include <map>
#include "include/subscription_status.h"
#include "include/price/price_field.h"

namespace bidfx_public_api::price::pixie
{
class Syncable
{
public:
    /**
     * Encodes a full price image for a single subscription.
     * @param sid the Subject ID (index within a subject set edition).
     * @param price the price fields.
     */
    virtual void PriceImage(uint32_t sid, std::map<std::string, PriceField> price) = 0;

    /**
     * Encodes a delta price update for a single subscription.
     * @param sid the Subject ID (index within a subject set edition).
     * @param price the price fields.
     */
    virtual void PriceUpdate(uint32_t sid, std::map<std::string, PriceField> price) = 0;

    /**
     * Encodes a delta price update for a single subscription.
     * @param sid the Subject ID (index within a subject set edition).
     * @param status the PublicAPI status code.
     * @param explanation a more detailed explanation of the problem.
     */
    virtual void PriceStatus(uint32_t sid, SubscriptionStatus status, std::string explanation) = 0;
};
}

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_SYNCABLE_H_
