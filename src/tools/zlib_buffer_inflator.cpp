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

#include <limits>
#include <sstream>
#include "assert.h"
#include "include/exceptions/compressor_exception.h"
#include "zlib_buffer_inflator.h"

namespace bidfx_public_api::tools
{

using bidfx_public_api::exceptions::CompressorException;

ZLIBBufferInflator::ZLIBBufferInflator()
{
    InitializeStream();
}

void ZLIBBufferInflator::InitializeStream()
{
    /* allocate inflate state */
    stream_.zalloc = Z_NULL;
    stream_.zfree = Z_NULL;
    stream_.opaque = Z_NULL;
    stream_.avail_in = 0;
    stream_.next_in = Z_NULL;
    int ret = inflateInit2(&stream_, -MAX_WBITS);
    if (ret != Z_OK)
    {
        std::stringstream ss;
        ss << "problem occurred while initializing stream: ZLIB_ERROR_CODE(" << ret << ")";
        throw CompressorException(ss.str());
    }
}

ZLIBBufferInflator::~ZLIBBufferInflator()
{
    // Free the dynamically created structures associated with the stream
    (void)inflateEnd(&stream_);
}

ByteBuffer ZLIBBufferInflator::Inflate(ByteBuffer& compressed_buffer)
{
    int ret;
    unsigned bytes_to_write;
    unsigned char in[compressed_buffer.ReadableBytes()];
    unsigned char out[ZLIB_INFLATOR_CHUNK_SIZE];
    ByteBuffer out_buffer;

    // decompress until deflate stream ends or end of buffer
    size_t bytes_to_read = compressed_buffer.ReadableBytes();
    if (bytes_to_read == 0)
    {
        return out_buffer;
    }

    size_t total_bytes_to_inflate = compressed_buffer.ReadBytes(in, bytes_to_read);

    for (size_t bytes_inflated = 0; bytes_inflated < total_bytes_to_inflate; )
    {
        uint32_t bytes_to_inflate = total_bytes_to_inflate > std::numeric_limits<uint32_t>::max() ? std::numeric_limits<uint32_t>::max() : (uint32_t) total_bytes_to_inflate;

        stream_.avail_in = bytes_to_inflate;
        stream_.next_in = in + bytes_inflated;

        // run inflate() on input until output buffer not full
        do
        {
            stream_.avail_out = ZLIB_INFLATOR_CHUNK_SIZE;
            stream_.next_out = out;
            ret = inflate(&stream_, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);
            switch (ret)
            {
                case Z_NEED_DICT:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void) inflateEnd(&stream_);
                    std::stringstream ss;
                    ss << "problem occurred while inflating: ZLIB_ERROR_CODE(" << ret << ")";
                    throw CompressorException(ss.str());
            }
            bytes_to_write = ZLIB_INFLATOR_CHUNK_SIZE - stream_.avail_out;
            out_buffer.WriteBytes(out, bytes_to_write);
        } while (stream_.avail_out == 0);

        if (ret == Z_STREAM_END)
        {
            // reset inflator
            (void) inflateEnd(&stream_);
            stream_ = z_stream();
            InitializeStream();
            break;
        }

        bytes_inflated += bytes_to_inflate;
    }
    return std::move(out_buffer);
}

}