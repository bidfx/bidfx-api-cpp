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

#ifndef PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_KEY_H_
#define PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_KEY_H_

#include <string>

namespace bidfx_public_api::price::subject
{

/**
 * This class defines constants for the common subject component keys.
 * @author Frazer Bennett Wilford
 */
class SubjectKey
{
public:
    static const std::string ASSET_CLASS;
    static const std::string BUY_SIDE_ACCOUNT;
    static const std::string CURRENCY;
    static const std::string CURRENCY_PAIR;
    static const std::string DEAL_TYPE;
    static const std::string EXCHANGE;
    static const std::string EXPIRY_DATE;
    static const std::string FAR_CURRENCY;
    static const std::string FAR_FIXING_DATE;
    static const std::string FAR_QUANTITY;
    static const std::string FAR_SETTLEMENT_DATE;
    static const std::string FAR_TENOR;
    static const std::string FIXING_CCY;
    static const std::string FIXING_DATE;
    static const std::string LEVEL;
    static const std::string LIQUIDITY_PROVIDER;
    static const std::string ON_BEHALF_OF;
    static const std::string PUT_CALL;
    static const std::string QUANTITY;
    static const std::string REQUEST_TYPE;
    static const std::string ROUTE;
    static const std::string ROWS;
    static const std::string SETTLEMENT_DATE;
    static const std::string SOURCE;
    static const std::string STRIKE;
    static const std::string SYMBOL;
    static const std::string TENOR;
    static const std::string USER;
};

}

#endif //PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_KEY_H_
