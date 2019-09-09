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

#ifndef PUBLIC_API_CPP_PRICE_CURRENCIES_H_
#define PUBLIC_API_CPP_PRICE_CURRENCIES_H_

#include <string>
#include <vector>

namespace bidfx_public_api::price
{

/**
 * This class provides a simple validator for currencies and currency pairs. It is not intended to be extensive.
 * @author Frazer Bennett Wilford
 */
class Currencies
{
private:
    static int Precedence(const std::string& ccy);

public:
    static const std::vector<std::string> CURRENCY_CODES;

    /**
     * Tests if a currency is a valid part of a currency pair.
     * @param ccy the currency
     * @param ccy_pair the currency pair
     * @return true if valid and false otherwise.
     */
    static bool IsValidCurrencyAndPair(const std::string& ccy, const std::string& ccy_pair);

    /**
     * Tests if a string is a valid six letter currency pair code.
     * @param ccy_pair the currency pair
     * @return true if valid and false otherwise.
     */
    static bool IsValidCurrencyPair(const std::string& ccy_pair);

    /**
     * Tests if a string is a valid three letter ISO currency code.
     * @param ccy the currency
     * @return true if valid and false otherwise.
     */
    static bool IsValidCurrency(const std::string& currency);
};

}


#endif //PUBLIC_API_CPP_PRICE_CURRENCIES_H_
