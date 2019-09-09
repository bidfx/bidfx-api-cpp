/**
 * Copyright 2018 2019 BidFX Systems Ltd. All rights reserved.
 */

#ifndef  PUBLIC_API_CPP_TOOLS_ZLIB_BUFFER_COMPRESSOR_H
#define  PUBLIC_API_CPP_TOOLS_ZLIB_BUFFER_COMPRESSOR_H

#include "lib/zlib/zlib.h"
#include "byte_buffer.h"
#include "buffer_compressor.h"

#define ZLIB_DEFLATOR_CHUNK_SIZE 16384

namespace bidfx_public_api::tools
{

class ZLIBBufferCompressor : public BufferCompressor
{
private:
    z_stream stream_;
    ByteBuffer buffer_;
    void Flush();
    int Deflate(bool flush);
public:
    ZLIBBufferCompressor(int level);
    ~ZLIBBufferCompressor();
    void Compress(ByteBuffer& fragment) override;
    void Compress(unsigned char* bytes, size_t offset, size_t length);
    ByteBuffer GetCompressed() override;
};

}

#endif //PUBLIC_API_CPP_TOOLS_ZLIB_BUFFER_COMPRESSOR_H
