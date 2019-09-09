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

#ifndef PUBLIC_API_CPP_PIXIE_PRICE_MESSAGES_MOCK_SYNCABLE_H_
#define PUBLIC_API_CPP_PIXIE_PRICE_MESSAGES_MOCK_SYNCABLE_H_

#include "gmock/gmock.h"
#include "src/price/provider/pixie/messages/syncable.h"

namespace bidfx_public_api::price::pixie
{

class MockSyncable : public Syncable
{
public:
    MOCK_METHOD2(PriceImage, void(uint32_t sid, std::map<std::string, PriceField> price));
    MOCK_METHOD2(PriceUpdate, void(uint32_t sid, std::map<std::string, PriceField> price));
    MOCK_METHOD3(PriceStatus, void(uint32_t sid, SubscriptionStatus status, std::string explanation));
};

}

#endif //PUBLIC_API_CPP_PIXIE_PRICE_MESSAGES_MOCK_SYNCABLE_H_
