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

#include <string>
#include "include/tools/java_time.h"

namespace bidfx_public_api::tools
{

int JavaTime::DayToGregorianDate(int day_number)
{
    int year = (int) ((10000L * day_number + 14780) / 3652425);
    int ddd = day_number - (365 * year + year / 4 - year / 100 + year / 400);
    if (ddd < 0)
    {
        --year;
        ddd = day_number - (365 * year + year / 4 - year / 100 + year / 400);
    }

    int mi = (100 * ddd + 52) / 3060;
    int month = (mi + 2) % 12 + 1;
    year += (mi + 2) / 12;
    int day = ddd - (mi * 306 + 5) / 10 + 1;
    return 10000 * year + 100 * month + day;
}

int JavaTime::ToIsoDate(long millis)
{
    return DayToGregorianDate((int) (millis / kDay) + kDaysToEpoch);
}

long JavaTime::ToIsoTime(long millis)
{
    long date = ToIsoDate(millis);
    date *= 100L;
    date += millis / kHour % 24;
    date *= 100L;
    date += millis / kMinute % 60;
    date *= 100000L;
    date += millis % kMinute;
    return date;
}

} // namespace bidfx_public_api::tools