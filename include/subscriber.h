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

#ifndef PUBLIC_API_CPP_SUBSCRIBER_H_
#define PUBLIC_API_CPP_SUBSCRIBER_H_

#include "include/price/subject/subject.h"
#include <memory>

namespace bidfx_public_api
{

using bidfx_public_api::price::subject::Subject;

/**
 * This interface is used by components that can subscribe to realtime pricing.
 * @author Frazer Bennett Wilford
 */
class Subscriber
{
public:
    /**
     * Subscribe to a realtime price stream.
     * @param subject the subject to subscribe on.
     * @param refresh flag that is set to true when the caller is attempting to refresh an existing subscription.
     */
    virtual void Subscribe(Subject subject, bool refresh) = 0;

    /**
     * Subscribe to a realtime price stream.
     * @param subject the subject to subscribe on.
     */
    virtual void Subscribe(Subject subject) = 0;

    /**
     * Unsubscribe from realtime price stream.
     * @param subject the subject to unsubscribe from.
     */
    virtual void Unsubscribe(Subject& subject) = 0;
};

}

#endif //PUBLIC_API_CPP_SUBSCRIBER_H_
