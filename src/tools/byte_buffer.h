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

#ifndef PUBLIC_API_CPP_TOOLS_BYTE_BUFFER_H_
#define PUBLIC_API_CPP_TOOLS_BYTE_BUFFER_H_

#include <cstdint>
#include <stddef.h>
#include <string>
#include "src/tools/network/ssl_client.h"

#define DEFAULT_MAX_CAPACITY 8192

namespace bidfx_public_api::tools
{

class ByteBuffer : public InputStream, public OutputStream
{
private:
    size_t reader_index_;
    size_t writer_index_;
    unsigned char* buf_;
    size_t capacity_;

    void CheckBufferHasData(size_t minimum_bytes) const;
    void EnsureWritableBytes(size_t min_writable_bytes);
    size_t WriteableBytes() const;
    void CompactIfNeededToWriteBytes(size_t size);
    size_t AvailableToWrite() const;
public:

    ByteBuffer(size_t size = DEFAULT_MAX_CAPACITY);
    ByteBuffer(const ByteBuffer& to_copy);
    ByteBuffer(ByteBuffer&& to_move);
    ByteBuffer& operator=(const ByteBuffer& to_copy);
    ByteBuffer& operator=(ByteBuffer&& to_move);
    ~ByteBuffer();
    unsigned char ReadByte() override;

    size_t ReadBytes(OutputStream& output_buf, size_t len);

    size_t ReadBytes(unsigned char* output_buf, size_t len) override;

    int32_t ReadInt();

    int64_t ReadLong();

    float ReadFloat();

    int16_t ReadShort();

    int32_t ReadMedium();

    double ReadDouble();

    uint8_t Read(size_t index);

    unsigned char GetNextByte();

    size_t WriteByte(const unsigned char data) override;

    size_t WriteBytes(ByteBuffer&& buffer);

    size_t WriteBytes(const unsigned char* input, size_t len) override;

    size_t WriteInt(const int32_t value);

    size_t WriteLong(const int64_t value);

    size_t WriteFloat(const float value);

    size_t WriteShort(const int16_t value);

    size_t WriteMedium(const int32_t value);

    size_t WriteDouble(const double value);

    size_t WriteString(const std::string& str) override;

    size_t ReadableBytes() const;

    void Clear();

    void Compact();

    uint8_t* ToArray() const;

    size_t Seek(size_t offset, unsigned char origin);

    void SkipBytes(size_t bytes_to_skip);

    std::string ToString() const;

    void SetByte(size_t index, uint8_t byte);

    const bool operator==(const ByteBuffer& other) const;

    const bool operator!=(const ByteBuffer& other) const;

    void Fill(unsigned char filler);
};

} // namespace bidfx_public_api::tools

#endif // PUBLIC_API_CPP_TOOLS_BYTE_BUFFER_H_
