/**
 * Copyright 2018 2019 BidFX Systems Ltd. All rights reserved.
 */

#include <limits>
#include <sstream>
#include "assert.h"
#include "include/exceptions/compressor_exception.h"
#include "zlib_buffer_compressor.h"

namespace bidfx_public_api::tools
{

using bidfx_public_api::exceptions::CompressorException;

ZLIBBufferCompressor::ZLIBBufferCompressor(int level)
{
    /* allocate deflate state */
    stream_.zalloc = Z_NULL;
    stream_.zfree = Z_NULL;
    stream_.opaque = Z_NULL;
    stream_.next_in = Z_NULL;
    stream_.avail_in = Z_NULL;
    int ret = deflateInit2(&stream_, level, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK)
    {
        std::stringstream ss;
        ss << "problem occurred while deflating init: ZLIB_ERROR_CODE(" << ret << ")";
        throw CompressorException(ss.str());
    }
}

ZLIBBufferCompressor::~ZLIBBufferCompressor()
{
    (void)deflateEnd(&stream_);
}

void ZLIBBufferCompressor::Compress(ByteBuffer &fragment)
{
    Compress(fragment.ToArray(), 0, fragment.ReadableBytes());
    fragment.Clear();
}

void ZLIBBufferCompressor::Compress(unsigned char *bytes, size_t offset, size_t length)
{
    for (size_t bytes_compressed = 0; bytes_compressed < length; )
    {
        uint32_t bytes_to_compress = length > std::numeric_limits<uint32_t>::max() ? std::numeric_limits<uint32_t>::max() : (uint32_t) length;
        stream_.avail_in = bytes_to_compress;
        stream_.next_in = bytes + offset + bytes_compressed;
        Deflate(false);
        bytes_compressed += bytes_to_compress;
    }
}

ByteBuffer ZLIBBufferCompressor::GetCompressed()
{
    Flush();
    ByteBuffer buffer(buffer_);
    buffer_.Clear();
    return buffer;
}

void ZLIBBufferCompressor::Flush()
{
    int len;
    do
    {
        len = Deflate(true);
    } while(len != 0);
}

int ZLIBBufferCompressor::Deflate(bool flush)
{
    int ret;
    unsigned have;
    unsigned char out[ZLIB_DEFLATOR_CHUNK_SIZE];
    stream_.avail_out = ZLIB_DEFLATOR_CHUNK_SIZE;
    stream_.next_out = out;

    // run deflate() on input until output buffer not full
    do
    {
        ret = deflate(&stream_, flush ? Z_SYNC_FLUSH : Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR)
        {
            (void) deflateEnd(&stream_);
            std::stringstream ss;
            ss << "problem occurred while deflating: ZLIB_ERROR_CODE(" << ret << ")";
            throw CompressorException(ss.str());
        }


        have = ZLIB_DEFLATOR_CHUNK_SIZE - stream_.avail_out;
        buffer_.WriteBytes(out, have);
    } while (stream_.avail_out == 0);
    return stream_.avail_in;
}
}