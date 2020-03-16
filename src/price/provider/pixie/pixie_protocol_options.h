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

#ifndef PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_PIXIE_PROTOCOL_OPTIONS_H_
#define PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_PIXIE_PROTOCOL_OPTIONS_H_

#include "../protocol_options.h"
#include <string>
#include <chrono>

namespace bidfx_public_api::price::pixie
{

class PixieProtocolOptions : public ProtocolOptions
{
private:
    int version_;
    std::chrono::milliseconds heartbeat_interval_;
    std::chrono::milliseconds idle_;
    int minti_;
public:
    explicit PixieProtocolOptions();

    ~PixieProtocolOptions() override = default;

    std::string GetName() const override;

    bool CompressSubscriptions() const override ;

    std::chrono::milliseconds GetHeartbeatInterval() const override ;

    int GetVersion() const override ;

    std::chrono::milliseconds GetIdleInterval() const override ;

    int GetMinti() const override ;

    std::string GetProtocolSignature() const override ;
};

} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PLUGIN_PIXIE_PIXIE_PROTOCOL_OPTIONS_H_
