/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include "include/price_update_event.h"

namespace bidfx_public_api
{
PriceUpdateEvent::PriceUpdateEvent(Subject subject,
                                   std::map<std::string, PriceField> allPriceFields,
                                   std::map<std::string, PriceField> changedPriceFields) :
                                   subject_(subject), allPriceFields_(allPriceFields), changedPriceFields_(changedPriceFields)
{}

const Subject& PriceUpdateEvent::GetSubject() const
{
    return subject_;
}

const std::map<std::string, PriceField>& PriceUpdateEvent::GetAllPriceFields() const
{
    return allPriceFields_;
}

const std::map<std::string, PriceField>& PriceUpdateEvent::GetChangedPriceFields() const
{
    return changedPriceFields_;
}
}