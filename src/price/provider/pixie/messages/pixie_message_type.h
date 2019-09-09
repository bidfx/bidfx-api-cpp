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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PIXIE_MESSAGE_TYPE_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PIXIE_MESSAGE_TYPE_H_

namespace bidfx_public_api::price::pixie
{

class PixieMessageType
{
public:
    static const unsigned char ACK;
    static const unsigned char DATA_DICTIONARY;
    static const unsigned char GRANT;
    static const unsigned char HEARTBEAT;
    static const unsigned char LOGIN;
    static const unsigned char PRICE_SYNC;
    static const unsigned char SUBSCRIPTION_SYNC;
    static const unsigned char WELCOME;
};

} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PIXIE_MESSAGE_TYPE_H_