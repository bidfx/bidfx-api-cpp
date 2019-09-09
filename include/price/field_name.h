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

#ifndef PUBLIC_API_CPP_PRICE_FIELD_NAME_H_
#define PUBLIC_API_CPP_PRICE_FIELD_NAME_H_

#include <string>

namespace bidfx_public_api::price
{

/**
 * Field name constants for typical fields that may be published through the price API.
 * No provider ever publishes more than a small subset of these fields.
 * @author Frazer Bennett Wilford
 */
class FieldName
{
    public:

    // The ask price. Type is double.
    static const std::string Ask;
    static const std::string Ask1;
    static const std::string Ask2;
    static const std::string Ask3;
    static const std::string Ask4;
    static const std::string Ask5;
    static const std::string Ask6;
    static const std::string Ask7;
    static const std::string Ask8;
    static const std::string Ask9;
    static const std::string Ask10;

    // The all in price of an ask. Type is double.
    static const std::string AskAllInPrice;

    // The all in ask price of the far leg. Type is double.
    static const std::string AskEndAllInPrice;

    // The ask forward points of the end leg of a swap. Type is double.  Used in FX swaps.
    static const std::string AskEndFwdPoints;

    // The ask size of the end leg of a swap. Type is long. Used in FX swaps.
    static const std::string AskEndSize;

    // The liquidity provider at level n of book in a level two quote, corresponding to the price at Askn.
    // Type is string.
    static const std::string AskFirm1;
    static const std::string AskFirm2;
    static const std::string AskFirm3;
    static const std::string AskFirm4;
    static const std::string AskFirm5;
    static const std::string AskFirm6;
    static const std::string AskFirm7;
    static const std::string AskFirm8;
    static const std::string AskFirm9;
    static const std::string AskFirm10;

    // The ask forward points as an offset from spot. Type is double. Used in FX quotes.
    static const std::string AskForwardPoints;
    static const std::string AskForwardPoints1;
    static const std::string AskForwardPoints2;
    static const std::string AskForwardPoints3;
    static const std::string AskForwardPoints4;
    static const std::string AskForwardPoints5;
    static const std::string AskForwardPoints6;
    static const std::string AskForwardPoints7;
    static const std::string AskForwardPoints8;
    static const std::string AskForwardPoints9;
    static const std::string AskForwardPoints10;

    // The number of levels on the Bid side of a Depth subscription. Type is integer.
    static const std::string AskLevels;

    // The ask size. Type is long.
    static const std::string AskSize;
    static const std::string AskSize1;
    static const std::string AskSize2;
    static const std::string AskSize3;
    static const std::string AskSize4;
    static const std::string AskSize5;
    static const std::string AskSize6;
    static const std::string AskSize7;
    static const std::string AskSize8;
    static const std::string AskSize9;
    static const std::string AskSize10;

    // The ask spot rate. Type is double. Used in FX quotes.
    static const std::string AskSpot;
    static const std::string AskSpot1;
    static const std::string AskSpot2;
    static const std::string AskSpot3;
    static const std::string AskSpot4;
    static const std::string AskSpot5;
    static const std::string AskSpot6;
    static const std::string AskSpot7;
    static const std::string AskSpot8;
    static const std::string AskSpot9;
    static const std::string AskSpot10;

    // The time of the ask price or size tick at depth n. Type is long.
    static const std::string AskTime1;
    static const std::string AskTime2;
    static const std::string AskTime3;
    static const std::string AskTime4;
    static const std::string AskTime5;
    static const std::string AskTime6;
    static const std::string AskTime7;
    static const std::string AskTime8;
    static const std::string AskTime9;
    static const std::string AskTime10;

    // The bid price. Type is double.
    static const std::string Bid;
    static const std::string Bid1;
    static const std::string Bid2;
    static const std::string Bid3;
    static const std::string Bid4;
    static const std::string Bid5;
    static const std::string Bid6;
    static const std::string Bid7;
    static const std::string Bid8;
    static const std::string Bid9;
    static const std::string Bid10;

    // The all in price of an bid. Type is double.
    static const std::string BidAllInPrice;

    // The all in bid price of the far leg. Type is double.
    static const std::string BidEndAllInPrice;

    // The bid forward points of the end leg of a swap. Type is double.  Used in FX swaps.
    static const std::string BidEndFwdPoints;

    // The bid size of the end leg of a swap. Type is long.  Used in FX swaps.
    static const std::string BidEndSize;

    // The liquidity provider at level n of book in a level two quote, corresponding to the price at Bidn.
    // Type is string.
    static const std::string BidFirm1;
    static const std::string BidFirm2;
    static const std::string BidFirm3;
    static const std::string BidFirm4;
    static const std::string BidFirm5;
    static const std::string BidFirm6;
    static const std::string BidFirm7;
    static const std::string BidFirm8;
    static const std::string BidFirm9;
    static const std::string BidFirm10;

    // The bid forward points as an offset from spot. Type is double.  Used in FX quotes.
    static const std::string BidForwardPoints;
    static const std::string BidForwardPoints1;
    static const std::string BidForwardPoints2;
    static const std::string BidForwardPoints3;
    static const std::string BidForwardPoints4;
    static const std::string BidForwardPoints5;
    static const std::string BidForwardPoints6;
    static const std::string BidForwardPoints7;
    static const std::string BidForwardPoints8;
    static const std::string BidForwardPoints9;
    static const std::string BidForwardPoints10;

    // The number of levels on the Bid side of a Depth subscription. Type is integer.
    static const std::string BidLevels;

    // The bid size. Type is long.
    static const std::string BidSize;
    static const std::string BidSize1;
    static const std::string BidSize2;
    static const std::string BidSize3;
    static const std::string BidSize4;
    static const std::string BidSize5;
    static const std::string BidSize6;
    static const std::string BidSize7;
    static const std::string BidSize8;
    static const std::string BidSize9;
    static const std::string BidSize10;

    // The bid spot rate. Type is double.
    static const std::string BidSpot;
    static const std::string BidSpot1;
    static const std::string BidSpot2;
    static const std::string BidSpot3;
    static const std::string BidSpot4;
    static const std::string BidSpot5;
    static const std::string BidSpot6;
    static const std::string BidSpot7;
    static const std::string BidSpot8;
    static const std::string BidSpot9;
    static const std::string BidSpot10;

    // The time of the bid price or size tick at depth n. Type is long.
    static const std::string BidTime1;
    static const std::string BidTime2;
    static const std::string BidTime3;
    static const std::string BidTime4;
    static const std::string BidTime5;
    static const std::string BidTime6;
    static const std::string BidTime7;
    static const std::string BidTime8;
    static const std::string BidTime9;
    static const std::string BidTime10;

    // The price ID used to uniquely identify a dealable price. Type is string.
    static const std::string PriceID;

    // The status code. Type is string.
    static const std::string Status;

    // The system time of the last price update within the pricing infrastructure.
    // This will normally be populated by the price server closest to the source of liquidity
    // but not necessarily the source itself. Should be to millisecond precession.
    // Type is long.
    static const std::string SystemTime;
};

}

#endif //PUBLIC_API_CPP_PRICE_FIELD_NAME_H_