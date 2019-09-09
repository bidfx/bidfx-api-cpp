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
#include "character_padding.h"

namespace bidfx_public_api::tools
{

CharacterPadding::CharacterPadding(char pad_char, unsigned int len) : pad_char_{pad_char}, length_{len < 0 ? 0 : len}
{
    std::stringstream ss;
    for (int i = 0; i < length_; i++)
    {
        ss << pad_char_;
    }

    constructed_padding_ = ss.str();
}

std::string CharacterPadding::ToString()
{
    return constructed_padding_;
}

}