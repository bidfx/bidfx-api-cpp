/**
 * Copyright 2018 2018 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_SUBSCRIPTION_STATUS_H
#define PUBLIC_API_CPP_SUBSCRIPTION_STATUS_H

#include <map>
#include "spdlog/fmt/ostr.h"

namespace bidfx_public_api
{

enum SubscriptionStatus
{
    /**
    * This status indicates that the subscription is pending a reply from the price service. This is the initial state of all new
    * subscriptions.
    */
    PENDING,

    /**
    * This status indicates that the subscription is OK. The price is considered to be good.
    */
    OK,

    /**
    * This status indicates that the subscription is stale. Data associated with a stale subscription should be treated as
    * indicative; it should never be use for trading. Most commonly a network connection failure results in a subscription being
    * marked as stale. The subscription will automatically revert to an OK status once the failure has been resolved.
    */
    STALE,

    /**
    * This status indicates that price flow has been cancelled by the feed. This status may be used by RFQ/RFS feeds to inform the
    * client of a premature end to the price flow. status.
    */
    CANCELLED,

    /**
    * This status indicates that price flow has been discontinued, perhaps only temporarily, by the feed. This status may be used
    * by RFQ/RFS feeds to inform the client of a temporary halt in the price flow. Should the flow continue then the status will
    * revert to OK.
    */
    DISCONTINUED,

    /**
    * This status indicates that access to the data associated with the subscription is prohibited because the user is not
    * authorised to use it.
    */
    PROHIBITED,

    /**
    * This status indicates that the subscription is unavailable via the price service. A subscription is unavailable when its
    * subject cannot be resolved by any of the configured price providers, ie there is no price feed that can handle the
    * subscription. It is possible that a price service reconfiguration can result in the resolution of a previously unavailable
    * subscription.
    */
    UNAVAILABLE,

    /**
    * This status indicates that the subscription has been rejected its external price feed (for example by Reuters or Bloomberg).
    * A subscription is normally be rejected because it does not map to a valid product on the feed. Most rejected subscriptions
    * remain in a rejected state until closed.
    */
    REJECTED,

    /**
    * This status indicates that the subscription request is has timed out. This state is normally used to indicate the price
    * service has abandoned the subscription request due to persistent communications problems or after waiting for a long time on
    * a response to come back from a remote feed.
    */
    TIMEOUT,

    /**
    * This status indicates that the subscription has been terminated by the price service due to a lengthy period of inactivity by
    * the publisher. This status is used by housekeeping functions within the price service that act as a preventative measure
    * against resource depletion that could result from clients that do no free up subscriptions when they are finished with.
    */
    INACTIVE,

    /**
    * This status indicates that the subscription cannot be satisfied by the price service due to resource exhaustion. This could
    * result from a lack of memory, threads or network bandwidth but more likely it is due to clients exceeding limits, imposed by
    * a feed, on the number of simultaneous subscriptions that the feed will allow.
    */
    EXHAUSTED,

    /**
    * This status indicates that the subscription is closed. This status is set when a subscription is closed and once set the
    * status will never again change for the duration of the subscription. A new subscription will be required to get a different
    * status for the same subject. For RFQ/RFS quotes this status marks the normal termination of the flow by the feed (usually due
    * to the time-out period expiring).
    */
    CLOSED,
};

static std::map<SubscriptionStatus, std::string> subscription_status_map = {
        {SubscriptionStatus::PENDING, "PENDING"},
        {SubscriptionStatus::OK, "OK"},
        {SubscriptionStatus::STALE, "STALE"},
        {SubscriptionStatus::CANCELLED, "CANCELLED"},
        {SubscriptionStatus::DISCONTINUED, "DISCONTINUED"},
        {SubscriptionStatus::PROHIBITED, "PROHIBITED"},
        {SubscriptionStatus::UNAVAILABLE, "UNAVAILABLE"},
        {SubscriptionStatus::REJECTED, "REJECTED"},
        {SubscriptionStatus::TIMEOUT, "TIMEOUT"},
        {SubscriptionStatus::INACTIVE, "INACTIVE"},
        {SubscriptionStatus::EXHAUSTED, "EXHAUSTED"},
        {SubscriptionStatus::CLOSED, "CLOSED"}
};

inline std::ostream& operator<<(std::ostream& os, const SubscriptionStatus& s)
{
    return os << subscription_status_map[s];
}

}

#endif //BIDFX_PUBLIC_API_SUBSCRIPTION_STATUS_H
