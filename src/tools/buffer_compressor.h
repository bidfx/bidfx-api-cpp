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

#ifndef PUBLIC_API_CPP_TOOLS_BUFFER_COMPRESSOR_H_
#define PUBLIC_API_CPP_TOOLS_BUFFER_COMPRESSOR_H_

#include <memory>
#include "byte_buffer.h"

namespace bidfx_public_api::tools
{

class BufferCompressor
{
public:
    /**
     * Compresses a buffer to the end of the compressed buffer. The fragment is then cleared ready for reuse.
     * @param fragment the fragment to append.
     */
    virtual void Compress(ByteBuffer& fragment) = 0;

    /**
     * Gets a copy of the compressed buffer containing the appended buffers and clear the larger buffer ready for its next use.
     */
    virtual ByteBuffer GetCompressed() = 0;
};

}
#endif //PUBLIC_API_CPP_TOOLS_BUFFER_COMPRESSOR_H_
