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

#ifndef PUBLIC_API_CPP_TOOLS_INPUT_STREAM_H_
#define PUBLIC_API_CPP_TOOLS_INPUT_STREAM_H_

#include <cstdio>

namespace bidfx_public_api::tools
{

class InputStream
{
public:
    virtual unsigned char ReadByte() = 0;
    virtual size_t ReadBytes(unsigned char* buf, size_t len) = 0;
};

}

#endif //PUBLIC_API_CPP_TOOLS_INPUT_STREAM_H_
