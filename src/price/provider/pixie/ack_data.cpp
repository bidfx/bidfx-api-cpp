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

#include <chrono>
#include <algorithm>
#include "ack_data.h"

namespace bidfx_public_api::price::pixie
{

AckData::AckData(long revision, long revision_time, long price_received_time, long handling_start_nano_time)
{
    revision_ = revision;
    revision_time_ = revision_time;
    price_received_time_ = price_received_time;
    handling_start_nano_time_ = handling_start_nano_time;
}

long AckData::GetRevision()
{
    return revision_;
}

bool AckData::IsReadyToBeSent()
{
    return ready_to_be_sent_;
}

void AckData::SetReadyToBeSent(bool ready_to_be_sent)
{
    this->ready_to_be_sent_ = ready_to_be_sent;
}

AckMessage AckData::ToAckMessage()
{
    long ack_time = std::chrono::system_clock::now().time_since_epoch().count();
    long handling_end_nano_time = std::chrono::high_resolution_clock::now().time_since_epoch().count(); // TODO: compatability issues to check
    return ToAckMessage(ack_time, handling_end_nano_time);
}

AckMessage AckData::ToAckMessage(long ack_time, long handling_end_nano_time)
{
    long handling_duration_nanos = std::max(0L, handling_end_nano_time - handling_start_nano_time_);
    long handling_duration_micros = (handling_duration_nanos + 500L) / 1000L;
    return AckMessage(revision_, revision_time_, price_received_time_, ack_time, handling_duration_micros);
}

AckData::AckData(const AckData &to_copy)
{
    revision_ = to_copy.revision_;
    revision_time_ = to_copy.revision_time_;
    price_received_time_ = to_copy.price_received_time_;
    handling_start_nano_time_ = to_copy.handling_start_nano_time_;
}

} // namespace bidfx_public_api::price::pixie