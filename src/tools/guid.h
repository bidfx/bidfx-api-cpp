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


#ifndef PUBLIC_API_CPP_TOOLS_GUID_H_
#define PUBLIC_API_CPP_TOOLS_GUID_H_

#include <cstdint>
#include <string>
#include "optional.h"

namespace bidfx_public_api::tools
{

/**
 * This class provides a globally unique identifier (GUID). The probability of two GUID's being equal is so small that it can be
 * considered inconceivable. A GUID is therefore an ideal way to uniquely identify an application in a distributed system.
 * @author Frazer Bennett Wilford
 */
class GUID
{
private:
    const size_t NUM_BYTES = 16;
    unsigned char* guid_;

    unsigned char* GenerateRandomBytes(size_t num_bytes);
public:

    /**
     * Creates a new GUID.
     */
    GUID();

    ~GUID();

    std::string ToString();
};

inline std::ostream & operator<<(std::ostream& str, GUID &v)
{
    return str << v.ToString();
}

} // namespace bidfx_public_api::tools

#endif //PUBLIC_API_CPP_TOOLS_GUID_H_
