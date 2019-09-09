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

#ifndef PUBLIC_API_CPP_TENOR_H_
#define PUBLIC_API_CPP_TENOR_H_

#include <string>
#include <map>

namespace bidfx_public_api {

/**
 * This interface provides access to standard tenors.
 * @author Frazer Bennett Wilford
 */
class Tenor
{
private:
    explicit Tenor(std::string tenor, int precedence);
    std::string tenor_code_;
    int precedence_;

    static std::map<std::string, const Tenor*> tenor_list_;

    static void CreateTenorList();

public:
    std::string ToString() const;

    /**
     * An integer value which is larger for Tenors which occur later in the future. Returns -1 for invalid tenors.
     * @return precedence value
     */
    int GetPrecedence() const;

    /**
     * Determines if the near tenor comes before the far tenor
     * @param near_tenor
     * @param far_tenor
     * @return true if the near tenor comes before the far tenor
     */
    static bool IsValidNearFarTenor(const Tenor& near_tenor, const Tenor& far_tenor);

    /**
     * Returns an instance of Tenor equivalent to the input string
     * @param tenor string of the tenor to convert into Tenor
     * @return the tenor that has been determined from the parameter
     */
    static const Tenor& GetTenorFromString(const std::string& tenor);

    /**
     * Gets a tenor for a non-standard broken date.
     */
    static const Tenor BROKEN_DATE;

    /**
     * Gets a standard tenor for settlement today.
     */
    static const Tenor TODAY;

    /**
     * Gets a standard tenor for settlement at spot-next (the day after spot).
     */
    static const Tenor TOMORROW;

    /**
     * Gets a standard tenor for settlement at spot.
     */
    static const Tenor SPOT;

    /**
     * Gets a standard tenor for settlement tomorrow.
     */
    static const Tenor SPOT_NEXT;
    static const Tenor IN_1_WEEK;
    static const Tenor IN_2_WEEKS;
    static const Tenor IN_3_WEEKS;
    static const Tenor IN_1_MONTH;
    static const Tenor IN_2_MONTHS;
    static const Tenor IN_3_MONTHS;
    static const Tenor IN_4_MONTHS;
    static const Tenor IN_5_MONTHS;
    static const Tenor IN_6_MONTHS;
    static const Tenor IN_7_MONTHS;
    static const Tenor IN_8_MONTHS;
    static const Tenor IN_9_MONTHS;
    static const Tenor IN_10_MONTHS;
    static const Tenor IN_11_MONTHS;
    static const Tenor IN_1_YEAR;
    static const Tenor IN_2_YEARS;
    static const Tenor IN_3_YEARS;
    static const Tenor IMM_MARCH;
    static const Tenor IMM_JUNE;
    static const Tenor IMM_SEPTEMBER;
    static const Tenor IMM_DECEMBER;
};

}

#endif //PUBLIC_API_CPP_TENOR_H_
