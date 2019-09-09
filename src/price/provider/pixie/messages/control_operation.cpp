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
#include "control_operation.h"

namespace bidfx_public_api::price::pixie
{

uint8_t ControlOperation::GetCode(ControlOperationEnum operation)
{
    if (operation == ControlOperationEnum::REFRESH)
    {
        return (uint8_t) 'R';
    }

    return (uint8_t) 'T';
}

ControlOperationEnum ControlOperation::FromCode(int code)
{
    if (code == 'R')
    {
        return ControlOperationEnum::REFRESH;
    } else if (code == 'T')
    {
        return ControlOperationEnum::TOGGLE;
    }
    throw std::invalid_argument("unknown subscription control type code '" + std::to_string(code) + "'");
}

std::string ControlOperation::ToString(ControlOperationEnum control)
{
    if (control == ControlOperationEnum::TOGGLE)
    {
        return "TOGGLE";
    }
    else if (control == ControlOperationEnum::REFRESH)
    {
        return "REFRESH";
    }

    return "NONE";
}

} // namespace bidfx_public_api::price::pixie