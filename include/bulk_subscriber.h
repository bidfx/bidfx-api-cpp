/**  Copyright 2019 BidFX

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

            http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */

#ifndef PUBLIC_API_CPP_PRICE_BULK_SUBSCRIBER_H_
#define PUBLIC_API_CPP_PRICE_BULK_SUBSCRIBER_H_

#include "include/subscriber.h"
#include "include/price/subject/subject_factory.h"

namespace bidfx_public_api
{

using bidfx_public_api::price::subject::SubjectFactory;

/**
 * This interface is used to subscribe and unsubscribe to pricing.
 * @author Frazer Bennett Wilford
 */
class BulkSubscriber : virtual public Subscriber
{
public:
    virtual ~BulkSubscriber() {}

    /**
     * Bulk unsubscribe to all subjects previously subscribed to.
     */
    virtual void UnsubscribeAll() = 0;

    /**
     * Bulk resubscribe to all subjects previously subscribed to.
     */
    virtual void ResubscribeAll() = 0;

    /**
     * Gets a factory for creating well formed price subjects.
     */
    virtual SubjectFactory Subjects() = 0;
};

}

#endif //PUBLIC_API_CPP_PRICE_BULK_SUBSCRIBER_H_
