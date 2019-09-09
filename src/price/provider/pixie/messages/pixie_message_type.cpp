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

#include "pixie_message_type.h"

namespace bidfx_public_api::price::pixie
{

const unsigned char PixieMessageType::ACK = 'A';
const unsigned char PixieMessageType::DATA_DICTIONARY = 'D';
const unsigned char PixieMessageType::GRANT = 'G';
const unsigned char PixieMessageType::HEARTBEAT = 'H';
const unsigned char PixieMessageType::LOGIN = 'L';
const unsigned char PixieMessageType::PRICE_SYNC = 'P';
const unsigned char PixieMessageType::SUBSCRIPTION_SYNC = 'S';
const unsigned char PixieMessageType::WELCOME = 'W';

} // namespace bidfx_public_api::price::pixie
