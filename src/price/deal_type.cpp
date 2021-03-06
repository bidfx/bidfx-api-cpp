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

#include "include/price/deal_type.h"

namespace bidfx_public_api::price
{

const std::string DealType::SPOT = "Spot";
const std::string DealType::OUTRIGHT = "Outright";
const std::string DealType::FORWARD = "Outright";
const std::string DealType::NDF = "NDF";
const std::string DealType::SWAP = "Swap";
const std::string DealType::NDS = "NDS";
const std::string DealType::OPTION = "Option";
const std::string DealType::NDO = "NDO";

}
