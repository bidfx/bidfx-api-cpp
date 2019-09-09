/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#include "include/subscription_status_event.h"

namespace bidfx_public_api
{
SubscriptionStatusEvent::SubscriptionStatusEvent(Subject subject, SubscriptionStatus status, std::string explanation) : subject_(subject), status_(status), explanation_(explanation)
{}

const Subject SubscriptionStatusEvent::GetSubject()
{
    return subject_;
}

const SubscriptionStatus SubscriptionStatusEvent::GetStatus()
{
    return status_;
}

const std::string SubscriptionStatusEvent::GetExplanation()
{
    return explanation_;
}

} // namespace public_api_bidfx