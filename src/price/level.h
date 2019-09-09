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

#ifndef PUBLIC_API_CPP_PRICE_LEVEL_H_
#define PUBLIC_API_CPP_PRICE_LEVEL_H_

#include <string>

namespace bidfx_public_api::price
{

/**
 * This class provides constants for levels of price detail that may be subscribed via the price service. We separate the pricing
 * for an instrument into different levels of detail. Most client application are only need basic level-one data. Those client that
 * need more detail can obtain it via a separate subscription. In practice the different levels of detail often come from different
 * price services of remote feeds.
 *
 * <p>These constants are provided for use in client applications so as to standardise price level name usage), they do not
 * represent a complete list of what is available and the price service need not implement them all.
 * @author Frazer Bennett Wilford
 */
class Level
{
public:
    /**
     * Level one prices with best bid/ask), sizes and so on.
     */
    static const std::string ONE;
};

}

#endif //PUBLIC_API_CPP_PRICE_LEVEL_H_
