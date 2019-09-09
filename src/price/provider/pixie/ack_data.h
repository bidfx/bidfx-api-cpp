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

#ifndef PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_ACKDATA_H_
#define PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_ACKDATA_H_

#include "src/price/provider/pixie/messages/ack_message.h"

namespace bidfx_public_api::price::pixie
{

class AckData
{
private:
    long revision_;
    long revision_time_;
    long price_received_time_;
    long handling_start_nano_time_;
    bool ready_to_be_sent_ = false;

public:
    AckData(long revision, long revision_time, long price_received_time, long handling_start_nano_time);

    AckData(const AckData& to_copy);

    long GetRevision();

    bool IsReadyToBeSent();

    void SetReadyToBeSent(bool ready_to_be_sent);

    AckMessage ToAckMessage();

    AckMessage ToAckMessage(long ack_time, long handling_end_nano_time);

};

} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_ACKDATA_H
