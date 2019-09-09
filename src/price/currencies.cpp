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

namespace bidfx_public_api::price
{

const std::vector<std::string> Currencies::CURRENCY_CODES = {
        "XAU", "XPT", "XPD", "XAG", // metals
        "XDR", // special drawing rights
        "EUR", "GBP", "AUD", "NZD", "USD", "CAD", "CHF", "NOK", "DKK", "SEK", // G10
        "CLF", "KWD", "BHD", "OMR", "JOD", "FKP", "GIP", "SHP", "KYD", "CHE", "CUC", "BSD", "PAB", "BMD", "CUP", "CHW",
        "SGD", "BND", "LYD", "AZN", "ANG", "AWG", "BAM", "BGN", "BYN", "BBD", "BZD", "FJD", "TOP", "TND", "GEL", "WST",
        "XCD", "BRL", "PGK", "MXV", "PEN", "TMT", "QAR", "ILS", "AED", "TRY", "SAR", "PLN", "GHS", "RON", "MYR", "SDG",
        "TTD", "CNH", "CNY", "BOB", "HRK", "GTQ", "SRD", "HKD", "SBD", "MOP", "TJS", "SVC", "MAD", "VEF", "BWP", "ZAR",
        "SCR", "NAD", "LSL", "SZL", "ARS", "ERN", "MVR", "EGP", "MXN", "MDL", "ETB", "HNL", "CZK", "UAH", "UYI", "UYU",
        "NIO", "TWD", "THB", "MUR", "GMD", "DOP", "PHP", "RUB", "MKD", "BOV", "BTN", "INR", "AFN", "KGS", "HTG", "MZN",
        "SSP", "BDT", "LRD", "NPR", "KES", "CVE", "PKR", "VUV", "DZD", "ISK",
        "JPY",
        "XPF", "RSD", "ALL", "JMD", "LKR", "AOA", "DJF", "GYD", "COU", "YER", "HUF", "KZT", "NGN", "ZWL", "MRO", "KMF",
        "AMD", "SYP", "SOS", "CRC", "XAF", "XOF", "CLP", "MWK", "RWF", "KPW", "KRW", "IQD", "MMK", "CDF", "LBP", "BIF",
        "TZS", "MNT", "COP", "MGA", "UGX", "UZS", "KHR", "ZMW", "PYG", "SLL", "LAK", "GNF", "IDR", "VND", "STD", "IRR",
        "XSU", "XUA"};


bool Currencies::IsValidCurrencyAndPair(const std::string& ccy, const std::string& ccy_pair)
{
    return (ccy_pair.length() > ccy.length() && // currency fits into currency pair
               (ccy_pair.compare(ccy_pair.length() - ccy.length(), ccy.length(), ccy) == 0 || // currency at end of currency pair
                ccy_pair.compare(0, ccy.length(), ccy) == 0)); // currency at start of currency pair
}

bool Currencies::IsValidCurrencyPair(const std::string& ccy_pair)
{
    if (!ccy_pair.empty())
    {
        if (ccy_pair.length() == 6)
        {
            int precedence1 = Precedence(ccy_pair.substr(0, 3));

            if (precedence1 != -1)
            {
                int precedence2 = Precedence(ccy_pair.substr(3, 6));

                if (precedence2 != -1)
                {
                    return precedence1 < precedence2;
                }
            }
        }
    }

    return false;
}

bool Currencies::IsValidCurrency(const std::string& ccy)
{
    return !ccy.empty() && ccy.length() == 3 && Precedence(ccy) != -1;
}

int Currencies::Precedence(const std::string& ccy)
{
    for (int i = 0; i < Currencies::CURRENCY_CODES.size(); i++)
    {
        if (CURRENCY_CODES.at(static_cast<unsigned long>(i)) == ccy)
        {
            return i;
        }
    }

    return -1;
}

}
