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

#include <sstream>
#include "include/tenor.h"
 
namespace bidfx_public_api
{

std::map<std::string, const Tenor*> Tenor::tenor_list_;

const Tenor Tenor::BROKEN_DATE = Tenor("BD", -1);
const Tenor Tenor::TODAY = Tenor("TOD", 1);
const Tenor Tenor::TOMORROW = Tenor("TOM", 2);
const Tenor Tenor::SPOT = Tenor("Spot", 3);
const Tenor Tenor::SPOT_NEXT = Tenor("S/N", 4);
const Tenor Tenor::IN_1_WEEK = Tenor("1W", 5);
const Tenor Tenor::IN_2_WEEKS = Tenor("2W", 6);
const Tenor Tenor::IN_3_WEEKS = Tenor("3W", 7);
const Tenor Tenor::IN_1_MONTH = Tenor("1M", 8);
const Tenor Tenor::IN_2_MONTHS = Tenor("2M", 9);
const Tenor Tenor::IN_3_MONTHS = Tenor("3M", 10);
const Tenor Tenor::IN_4_MONTHS = Tenor("4M", 11);
const Tenor Tenor::IN_5_MONTHS = Tenor("5M", 12);
const Tenor Tenor::IN_6_MONTHS = Tenor("6M", 13);
const Tenor Tenor::IN_7_MONTHS = Tenor("7M", 14);
const Tenor Tenor::IN_8_MONTHS = Tenor("8M", 15);
const Tenor Tenor::IN_9_MONTHS = Tenor("9M", 16);
const Tenor Tenor::IN_10_MONTHS = Tenor("10M", 17);
const Tenor Tenor::IN_11_MONTHS = Tenor("11M", 18);
const Tenor Tenor::IN_1_YEAR = Tenor("1Y", 19);
const Tenor Tenor::IN_2_YEARS = Tenor("2Y", 21);
const Tenor Tenor::IN_3_YEARS = Tenor("3Y", 23);
const Tenor Tenor::IMM_MARCH = Tenor("IMMH", -1);
const Tenor Tenor::IMM_JUNE = Tenor("IMMM", -1);
const Tenor Tenor::IMM_SEPTEMBER = Tenor("IMMU", -1);
const Tenor Tenor::IMM_DECEMBER = Tenor("IMMZ", -1);

Tenor::Tenor(std::string tenor, int precedence)
{
    tenor_code_ = tenor;
    precedence_ = precedence;
}

std::string Tenor::ToString() const
{
    return tenor_code_;
}

int Tenor::GetPrecedence() const
{
    return precedence_;
}

bool Tenor::IsValidNearFarTenor(const Tenor& near_tenor, const Tenor& far_tenor)
{
    return (near_tenor.GetPrecedence() < far_tenor.GetPrecedence() || near_tenor.GetPrecedence() == -1 || far_tenor.GetPrecedence() == -1);
}

const Tenor& Tenor::GetTenorFromString(const std::string &tenor)
{
    if (tenor_list_.empty())
    {
        CreateTenorList();
    }

    if (tenor_list_.count(tenor))
    {
        return *tenor_list_[tenor];
    }

    std::stringstream ss;
    ss << "Tenor " << tenor << " does not exist" << std::endl;
    throw std::invalid_argument(ss.str());
}

void Tenor::CreateTenorList()
{
    tenor_list_[Tenor::BROKEN_DATE.ToString()] = &Tenor::BROKEN_DATE;
    tenor_list_[Tenor::TODAY.ToString()] = &Tenor::TODAY;
    tenor_list_[Tenor::TOMORROW.ToString()] = &Tenor::TOMORROW;
    tenor_list_[Tenor::SPOT.ToString()] = &Tenor::SPOT;
    tenor_list_[Tenor::SPOT_NEXT.ToString()] = &Tenor::SPOT_NEXT;
    tenor_list_[Tenor::IN_1_WEEK.ToString()] = &Tenor::IN_1_WEEK;
    tenor_list_[Tenor::IN_2_WEEKS.ToString()] = &Tenor::IN_2_WEEKS;
    tenor_list_[Tenor::IN_3_WEEKS.ToString()] = &Tenor::IN_3_WEEKS;
    tenor_list_[Tenor::IN_1_MONTH.ToString()] = &Tenor::IN_1_MONTH;
    tenor_list_[Tenor::IN_2_MONTHS.ToString()] = &Tenor::IN_2_MONTHS;
    tenor_list_[Tenor::IN_3_MONTHS.ToString()] = &Tenor::IN_3_MONTHS;
    tenor_list_[Tenor::IN_4_MONTHS.ToString()] = &Tenor::IN_4_MONTHS;
    tenor_list_[Tenor::IN_5_MONTHS.ToString()] = &Tenor::IN_5_MONTHS;
    tenor_list_[Tenor::IN_6_MONTHS.ToString()] = &Tenor::IN_6_MONTHS;
    tenor_list_[Tenor::IN_7_MONTHS.ToString()] = &Tenor::IN_7_MONTHS;
    tenor_list_[Tenor::IN_8_MONTHS.ToString()] = &Tenor::IN_8_MONTHS;
    tenor_list_[Tenor::IN_9_MONTHS.ToString()] = &Tenor::IN_9_MONTHS;
    tenor_list_[Tenor::IN_10_MONTHS.ToString()] = &Tenor::IN_10_MONTHS;
    tenor_list_[Tenor::IN_11_MONTHS.ToString()] = &Tenor::IN_11_MONTHS;
    tenor_list_[Tenor::IN_1_YEAR.ToString()] = &Tenor::IN_1_YEAR;
    tenor_list_[Tenor::IN_2_YEARS.ToString()] = &Tenor::IN_2_YEARS;
    tenor_list_[Tenor::IN_3_YEARS.ToString()] = &Tenor::IN_3_YEARS;
    tenor_list_[Tenor::IMM_MARCH.ToString()] = &Tenor::IMM_MARCH;
    tenor_list_[Tenor::IMM_JUNE.ToString()] = &Tenor::IMM_JUNE;
    tenor_list_[Tenor::IMM_SEPTEMBER.ToString()] = &Tenor::IMM_SEPTEMBER;
    tenor_list_[Tenor::IMM_DECEMBER.ToString()] = &Tenor::IMM_DECEMBER;
}

}