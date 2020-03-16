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

#include <sstream>
#include "pixie_protocol_options.h"
#include "pixie_version.h"

namespace bidfx_public_api::price::pixie
{

PixieProtocolOptions::PixieProtocolOptions() : heartbeat_interval_(std::chrono::milliseconds(15000)), idle_(std::chrono::milliseconds(120000)), minti_(0), version_(PixieVersion::CURRENT_VERSION)
{
}

std::string PixieProtocolOptions::GetName() const
{
    return "Pixie";
}

bool PixieProtocolOptions::CompressSubscriptions() const
{
    return false;
}

std::chrono::milliseconds PixieProtocolOptions::GetHeartbeatInterval() const
{
    return heartbeat_interval_;
}

std::string PixieProtocolOptions::GetProtocolSignature() const
{
    std::stringstream signature;
    signature << "pixie://localhost?version=" << version_ << "&heartbeat=" << heartbeat_interval_.count() / 1000 << "&idle=" << idle_.count() / 1000;

    if (minti_ != 0)
    {
        signature << "&minti=" << minti_;
    }

    std::string ret = signature.str();

    return ret;
}

int PixieProtocolOptions::GetVersion() const
{
    return version_;
}

std::chrono::milliseconds PixieProtocolOptions::GetIdleInterval() const
{
    return idle_;
}

int PixieProtocolOptions::GetMinti() const
{
    return minti_;
}

} // namespace bidfx_public_api::price::pixie


