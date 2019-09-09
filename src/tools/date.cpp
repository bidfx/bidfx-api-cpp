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

#include <stdexcept>
#include <sstream>
#include "include/tools/date.h"

namespace bidfx_public_api::tools
{

Date::Date(int year, int month, int day)
{
    if (IsValidDate(year, month, day))
    {
        year_ = year;
        month_ = month;
        day_ = day;
    }
    else
    {
        std::string error = "Date is invalid: ";

        throw std::invalid_argument(error + ToString());
    }
}

bool Date::IsValidDate(int year, int month, int day)
{
    switch (month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if (day > 31)
            {
                return false;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if (day > 30)
            {
                return false;
            }
            break;
        case 2:
            if (day > 29)
            {
                return false;
            }
            break;
        default:
            return false;
    }

    return (year < 3000 && year > 2017);
}

std::string Date::ToString()
{
    std::stringstream ss;

    ss << year_;
    PadSingleDigitIntsIntoStream(ss, month_);
    PadSingleDigitIntsIntoStream(ss, day_);

    return ss.str();
}

void Date::PadSingleDigitIntsIntoStream(std::stringstream& ss, int val)
{
    if (val < 10)
    {
        ss << "0" << val;
    }
    else
    {
        ss << val;
    }
}


}