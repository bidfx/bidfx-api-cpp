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


#ifndef PUBLIC_API_CPP_TOOLS_JAVA_TIME_H_
#define PUBLIC_API_CPP_TOOLS_JAVA_TIME_H_

#include <ctime>

namespace bidfx_public_api::tools
{

class JavaTime
{

private:
    static const long kMillisecond = 1L;
    static const long kSecond = 1000 * kMillisecond;
    static const long kMinute = 60 * kSecond;
    static const long kHour = 60 * kMinute;
    static const long kDay = 24 * kHour;
    static const int kDaysToEpoch = 719468;

    static int DayToGregorianDate(int day_number);

public:
    static int ToIsoDate(long millis);
    static long ToIsoTime(long millis);
};

} // namespace bidfx_public_api::tools

#endif //PUBLIC_API_CPP_TOOLS_JAVA_TIME_H_
