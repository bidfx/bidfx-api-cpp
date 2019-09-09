/**
 * Copyright 2019 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_PRICE_SUBSCRIPTION_H_
#define PUBLIC_API_CPP_PRICE_SUBSCRIPTION_H_

#include <string>
#include <map>
#include "include/price/subject/subject.h"
#include "include/subscription_status.h"
#include "include/price/price_field.h"

namespace bidfx_public_api::price
{

using bidfx_public_api::price::subject::Subject;

class Subscription
{
private:
    SubscriptionStatus status_;
    std::map<std::string, PriceField> price_fields_;
    Subject subject_;
public:
    Subscription(Subject subject);
    ~Subscription();
    Subscription(const Subscription& to_copy);
    Subscription(Subscription&& to_move);
    Subscription& operator=(const Subscription& to_copy);
    Subscription& operator=(Subscription&& to_move);

    const SubscriptionStatus& GetStatus() const;
    void SetStatus(const SubscriptionStatus status);
    const std::map<std::string, PriceField>& GetAllPriceFields() const;
    const Subject& GetSubject() const;

    void MergePriceMap(const std::map<std::string, PriceField>& price_update, bool replace_all_fields);
    void ClearPriceFields();
};
}

#endif //PUBLIC_API_CPP_PRICE_SUBSCRIPTION_H_
