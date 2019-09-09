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

#include "src/price/currencies.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::price::subject
{

using bidfx_public_api::price::Currencies;

/*****************
 * Testing Individual Currencies
 *****************/

TEST(CurrenciesTest, test_g10_currencies_are_valid)
{
    ASSERT_TRUE(Currencies::IsValidCurrency("USD"));
    ASSERT_TRUE(Currencies::IsValidCurrency("JPY"));
    ASSERT_TRUE(Currencies::IsValidCurrency("EUR"));
    ASSERT_TRUE(Currencies::IsValidCurrency("GBP"));
    ASSERT_TRUE(Currencies::IsValidCurrency("CHF"));
    ASSERT_TRUE(Currencies::IsValidCurrency("CAD"));
    ASSERT_TRUE(Currencies::IsValidCurrency("AUD"));
    ASSERT_TRUE(Currencies::IsValidCurrency("NZD"));
    ASSERT_TRUE(Currencies::IsValidCurrency("NOK"));
    ASSERT_TRUE(Currencies::IsValidCurrency("SEK"));
}

TEST(CurrenciesTest, test_metals_are_valid)
{
    ASSERT_TRUE(Currencies::IsValidCurrency("XAU"));
    ASSERT_TRUE(Currencies::IsValidCurrency("XAG"));
    ASSERT_TRUE(Currencies::IsValidCurrency("XPD"));
    ASSERT_TRUE(Currencies::IsValidCurrency("XPT"));
}

TEST(CurrenciesTest, test_currency_pairs_do_not_validate_as_a_currency)
{
    ASSERT_FALSE(Currencies::IsValidCurrency("EURGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrency("EURJPY"));
    ASSERT_FALSE(Currencies::IsValidCurrency("AUDNZD"));
    ASSERT_FALSE(Currencies::IsValidCurrency("USDEUR"));
}

TEST(CurrenciesTest, test_empty_string_is_not_valid_currency)
{
    ASSERT_FALSE(Currencies::IsValidCurrency(""));
}

/*****************
 * Testing Currency Pairs
 *****************/

TEST(CurrenciesTest, test_g10_currency_pairs_are_valid)
{
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("EURGBP"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("EURAUD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("EURNZD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("EURUSD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("EURCAD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("EURCHF"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("EURNOK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("EURDKK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("EURSEK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("EURJPY"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("GBPAUD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("GBPNZD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("GBPUSD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("GBPCAD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("GBPCHF"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("GBPNOK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("GBPDKK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("GBPSEK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("GBPJPY"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("AUDNZD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("AUDUSD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("AUDCAD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("AUDCHF"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("AUDNOK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("AUDDKK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("AUDSEK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("AUDJPY"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("NZDUSD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("NZDCAD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("NZDCHF"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("NZDNOK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("NZDDKK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("NZDSEK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("NZDJPY"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("USDCAD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("USDCHF"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("USDNOK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("USDDKK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("USDSEK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("USDJPY"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("CADCHF"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("CADNOK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("CADDKK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("CADSEK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("CADJPY"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("CHFNOK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("CHFDKK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("CHFSEK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("CHFJPY"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("NOKDKK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("NOKSEK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("NOKJPY"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("DKKSEK"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("DKKJPY"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("SEKJPY"));
}

TEST(CurrenciesTest, test_metal_dollar_is_valid)
{
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("XAUUSD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("XAGUSD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("XPDUSD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("XPTUSD"));
}

TEST(CurrenciesTest, test_metal_vs_majors_are_valid)
{
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("XAUGBP"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("XAGGBP"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("XPDEUR"));
    ASSERT_TRUE(Currencies::IsValidCurrencyPair("XPTZAR"));
}

TEST(CurrenciesTest, test_inverse_pairs_are_not_valid)
{
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("GBPEUR"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("AUDEUR"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("NZDEUR"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("USDEUR"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CADEUR"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CHFEUR"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("NOKEUR"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("DKKEUR"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("SEKEUR"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("JPYEUR"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("AUDGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("NZDGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("USDGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CADGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CHFGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("NOKGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("DKKGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("SEKGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("JPYGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("NZDAUD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("USDAUD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CADAUD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CHFAUD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("NOKAUD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("DKKAUD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("SEKAUD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("JPYAUD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("USDNZD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CADNZD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CHFNZD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("NOKNZD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("DKKNZD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("SEKNZD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("JPYNZD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CADUSD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CHFUSD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("NOKUSD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("DKKUSD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("SEKUSD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("JPYUSD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("CHFCAD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("NOKCAD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("DKKCAD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("SEKCAD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("JPYCAD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("NOKCHF"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("DKKCHF"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("SEKCHF"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("JPYCHF"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("DKKNOK"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("SEKNOK"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("JPYNOK"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("SEKDKK"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("JPYDKK"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("JPYSEK"));
}

TEST(CurrenciesTest, test_empty_string_is_not_valid_currency_pair)
{
    ASSERT_FALSE(Currencies::IsValidCurrencyPair(""));
}

TEST(CurrenciesTest, test_currency_is_not_valid_currency_pair)
{
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("GBP"));
}

TEST(CurrenciesTest, test_EBS_code_is_not_valid_currency_pair)
{
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("EUR/USD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("GBP:USD"));
}

TEST(CurrenciesTest, test_misc_is_not_currency_pair)
{
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("$"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("eurusd"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("EuroDollar"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("US$"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("cable"));
    ASSERT_FALSE(Currencies::IsValidCurrencyPair("what is this?"));
}

/*****************
 * Testing Currency & Currency Combinations
 *****************/

TEST(CurrenciesTest, test_base_currencies_are_valid)
{
    ASSERT_TRUE(Currencies::IsValidCurrencyAndPair("EUR", "EURGBP"));
    ASSERT_TRUE(Currencies::IsValidCurrencyAndPair("EUR", "EURUSD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyAndPair("GBP", "GBPAUD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyAndPair("XAU", "XAUUSD"));
}

TEST(CurrenciesTest, test_term_currencies_are_valid)
{
    ASSERT_TRUE(Currencies::IsValidCurrencyAndPair("GBP", "EURGBP"));
    ASSERT_TRUE(Currencies::IsValidCurrencyAndPair("USD", "EURUSD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyAndPair("AUD", "GBPAUD"));
    ASSERT_TRUE(Currencies::IsValidCurrencyAndPair("USD", "XAUUSD"));
}

TEST(CurrenciesTest, test_three_char_substrings_are_invalid)
{
    ASSERT_FALSE(Currencies::IsValidCurrencyAndPair("URG", "EURGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyAndPair("RGB", "EURGBP"));
}

TEST(CurrenciesTest, test_non_base_or_terms_currencies_are_invalid)
{
    ASSERT_FALSE(Currencies::IsValidCurrencyAndPair("NZD", "EURGBP"));
    ASSERT_FALSE(Currencies::IsValidCurrencyAndPair("USD", "AUDJPY"));
    ASSERT_FALSE(Currencies::IsValidCurrencyAndPair("CHF", "EURUSD"));
    ASSERT_FALSE(Currencies::IsValidCurrencyAndPair("MXN", "XAUUSD"));
}

}