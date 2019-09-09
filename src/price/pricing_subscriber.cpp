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

#include "pricing_subscriber.h"

namespace bidfx_public_api::price
{

PricingSubscriber::PricingSubscriber(PricingSession* session, UserInfo* user_info) : subjects_(user_info)
{
    session_ = session;
    user_info_ = user_info;
}


void PricingSubscriber::Subscribe(Subject subject, bool refresh)
{
    session_->Subscribe(subject, refresh);
}

void PricingSubscriber::Subscribe(Subject subject)
{
    session_->Subscribe(subject, false);
}

void PricingSubscriber::Unsubscribe(Subject& subject)
{
    session_->Unsubscribe(subject);
}

void PricingSubscriber::UnsubscribeAll()
{

}

void PricingSubscriber::ResubscribeAll()
{

}

SubjectFactory PricingSubscriber::Subjects()
{
    return subjects_;
}


}