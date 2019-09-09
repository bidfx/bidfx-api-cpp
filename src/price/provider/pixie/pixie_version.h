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

#ifndef PUBLIC_API_CPP_PRICE_PIXIE_PIXIE_VERSION_H_
#define PUBLIC_API_CPP_PRICE_PIXIE_PIXIE_VERSION_H_

#include "lib/logging/spdlog/spdlog.h"
#include "lib/logging/spdlog/logger.h"

namespace bidfx_public_api::price::pixie
{

class PixieVersion
{
private:
    static std::shared_ptr<spdlog::logger> Log;

    static void EnsureLoggerIsActive();

public:
    /**
     * Version 1 (release 19 December 2014 with publicapi-slip-plugin 1.10.8.4) provided the first attempt at improving on TickTalk
     * the wire format of Highway for FX subscribers.
     */
    static const int VERSION1;

    /**
     * Version 2 (release February 2015) introduced subscription controls (refresh and toggle) on SubscriptionSyncs and client
     * handling time on AckMessages.
     */
    static const int VERSION2;

    /**
     * Version 3 (April 2016) introduced grid price updates.
     */
    static const int VERSION3;

    /**
     * Version 4 (October 2018) introduced product serial keys.
     */
    static const int VERSION4;

    /**
     * The current protocol version.
     */
    static const int CURRENT_VERSION;

    static int ServerNegotiateVersion(int client_version);

    static int ServerNegotiateVersion(int client_version, int server_from, int server_to);

    static void ClientVerifyVersion(int server_version);

    static void ClientVerifyVersion(int server_version, int client_from, int client_to);
};


}


#endif //PUBLIC_API_CPP_PRICE_PIXIE_PIXIE_VERSION_H_
