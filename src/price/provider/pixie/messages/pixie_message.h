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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PIXIE_MESSAGE_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PIXIE_MESSAGE_H_

#include <memory>
#include "src/tools/byte_buffer.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::ByteBuffer;

/**
 * This interface define a basic message in the Pixie protocol
 * @author Frazer Bennett Wilford
 */
class PixieMessage
{
public:
    /**
     * Encodes this message and returns it as buffer.
     * @return encoded buffer
     */
    virtual ByteBuffer Encode(int version) const = 0;

    virtual const std::string ToString() = 0;
};

inline std::ostream & operator<<(std::ostream & str, PixieMessage & v)
{
    str << v.ToString();
    return str;
}

} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_PIXIE_MESSAGES_PIXIE_MESSAGE_H_
