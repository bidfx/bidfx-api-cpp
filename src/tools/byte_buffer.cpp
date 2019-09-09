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

#include <iostream>
#include <cstring>
#include <memory>
#include <sstream>
#include <vector>
#include "byte_buffer.h"

namespace bidfx_public_api::tools
{

ByteBuffer::ByteBuffer(size_t size)
{
    reader_index_ = 0;
    writer_index_ = 0;
    buf_ = new unsigned char[size];
    capacity_ = size;
}

ByteBuffer::ByteBuffer(const ByteBuffer& to_copy)
{
    buf_ = new unsigned char[to_copy.capacity_];
    reader_index_ = 0;
    writer_index_ = to_copy.writer_index_ - to_copy.reader_index_;
    capacity_ = to_copy.capacity_;
    memcpy(buf_, to_copy.buf_ + to_copy.reader_index_, writer_index_ * sizeof(unsigned char));
}

ByteBuffer::ByteBuffer(ByteBuffer&& to_move)
{
    buf_ = to_move.buf_;
    to_move.buf_ = nullptr;
    reader_index_ = to_move.reader_index_;
    writer_index_ = to_move.writer_index_;
    capacity_ = to_move.capacity_;
}

ByteBuffer& ByteBuffer::operator=(const bidfx_public_api::tools::ByteBuffer& to_copy)
{
    delete buf_;
    buf_ = new unsigned char[to_copy.capacity_];
    reader_index_ = 0;
    writer_index_ = to_copy.writer_index_ - to_copy.reader_index_;
    capacity_ = to_copy.capacity_;
    memcpy(buf_, to_copy.buf_+to_copy.reader_index_, writer_index_);
    return *this;
}

ByteBuffer& ByteBuffer::operator=(bidfx_public_api::tools::ByteBuffer&& to_move)
{
    delete buf_;
    buf_ = to_move.buf_;
    to_move.buf_ = nullptr;
    reader_index_ = to_move.reader_index_;
    writer_index_ = to_move.writer_index_;
    capacity_ = to_move.capacity_;
    return *this;
}

ByteBuffer::~ByteBuffer()
{
    delete buf_;
}

unsigned char ByteBuffer::ReadByte()
{
    if (reader_index_ == writer_index_)
    {
        std::stringstream ss;
        ss << "Readable byte limit exceeded: " << reader_index_;
        throw std::out_of_range(ss.str());
    }
    return GetNextByte();
}

size_t ByteBuffer::ReadBytes(OutputStream& output_buf, size_t len)
{
    CheckBufferHasData(len);
    size_t num_bytes = 0;
    for (; num_bytes < len; num_bytes++)
    {
        output_buf.WriteByte(GetNextByte());
    }
    return num_bytes;
}

size_t ByteBuffer::ReadBytes(unsigned char* output_buf, size_t len)
{
    CheckBufferHasData(len);
    size_t num_bytes = 0;
    for (; num_bytes < len; num_bytes++)
    {
        unsigned char byte = GetNextByte();
        output_buf[num_bytes] = byte;
    }
    return num_bytes;
}

unsigned char ByteBuffer::GetNextByte()
{
    CheckBufferHasData(1);
    unsigned char data = Read(reader_index_);
    reader_index_ += sizeof(data);
    return data;
}

unsigned char ByteBuffer::Read(size_t index)
{
    if (index + sizeof(uint8_t) <= capacity_)
    {
        return (buf_[index]);
    }
    return 0;
}

size_t ByteBuffer::WriteByte(const unsigned char data)
{
    EnsureWritableBytes(1);

    buf_[writer_index_++] = data;
    return 1;
}

size_t ByteBuffer::WriteBytes(ByteBuffer&& buffer)
{
    return WriteBytes(buffer.ToArray(), buffer.ReadableBytes());
}

size_t ByteBuffer::WriteBytes(const unsigned char* input, size_t len)
{
    EnsureWritableBytes(len);

    memcpy(buf_ + writer_index_, input, len);
    writer_index_ += len;

    return len;
}

size_t ByteBuffer::WriteInt(const int32_t value)
{
    uint32_t i = *(uint32_t*)&value;
    unsigned char bytes[4];
    bytes[0] = (unsigned char) (i >> 24);
    bytes[1] = (unsigned char) (i >> 16);
    bytes[2] = (unsigned char) (i >> 8);
    bytes[3] = (unsigned char) i;
    return WriteBytes(bytes, 4);
}

int32_t ByteBuffer::ReadInt()
{
    unsigned char bytes[4];
    ReadBytes(bytes, 4);
    uint32_t value =
            ((uint32_t) bytes[0] << 24) |
            ((uint32_t) bytes[1] << 16) |
            ((uint32_t) bytes[2] << 8) |
            ((uint32_t) bytes[3] & 0xff);
    return *(int32_t*)&value;
}

size_t ByteBuffer::WriteString(const std::string& str)
{
    size_t str_len = str.length();
    return WriteBytes(reinterpret_cast<const unsigned char*>(str.data()), str_len);
}

size_t ByteBuffer::ReadableBytes() const
{
    return writer_index_ - reader_index_;
}

void ByteBuffer::Clear()
{
    reader_index_ = 0;
    writer_index_ = 0;
}

void ByteBuffer::Compact()
{
    int count = 0;
    for (size_t i = reader_index_; i < writer_index_; i++)
    {
        buf_[count] = buf_[i];
        count++;
    }
    writer_index_ -= reader_index_;
    reader_index_ = 0;
}

void ByteBuffer::CompactIfNeededToWriteBytes(size_t len)
{
    if (len <= AvailableToWrite())
    {
        return;
    }

    Compact();
}

void ByteBuffer::Fill(uint8_t filler)
{
    writer_index_ = capacity_;
    memset(buf_, filler, capacity_);
}

size_t ByteBuffer::AvailableToWrite() const
{
    return capacity_ - writer_index_;
}

unsigned char* ByteBuffer::ToArray() const
{
    unsigned char* array = new unsigned char[ReadableBytes()];
    int count = 0;
    for (size_t i = reader_index_; i < writer_index_; i++)
    {
        array[count] = buf_[i];
        count++;
    }
    return array;
}

size_t ByteBuffer::Seek(size_t offset, uint8_t origin)
{
    switch (origin)
    {
        case 0:
            reader_index_ = offset;
            break;
        case 1:
            reader_index_ += offset;
            break;
    }
}

void ByteBuffer::SkipBytes(size_t bytes_to_skip)
{
    CheckBufferHasData(bytes_to_skip);
    reader_index_ += bytes_to_skip;
}

std::string ByteBuffer::ToString() const
{
    std::string string(buf_ + reader_index_, buf_ + writer_index_);
    return string;
}

void ByteBuffer::CheckBufferHasData(size_t minimum_bytes) const
{
    if (ReadableBytes() < minimum_bytes)
    {
        throw std::overflow_error("Buffer does not have enough data");
    }
}

void ByteBuffer::EnsureWritableBytes(size_t min_writable_bytes)
{
    CompactIfNeededToWriteBytes(min_writable_bytes);
    if (min_writable_bytes <= AvailableToWrite())
    {
        return;
    }

    size_t new_capacity;
    if (capacity_ == 0)
    {
        new_capacity = 1;
    }
    else
    {
        new_capacity = capacity_;
    }

    size_t min_new_capacity = writer_index_ + min_writable_bytes;
    while (new_capacity < min_new_capacity)
    {
        new_capacity <<= 1;
        if (new_capacity == 0) { throw std::overflow_error("Error: size exceeded 2GB"); }
    }

    unsigned char* copy_buff = new unsigned char[new_capacity];
    std::memcpy(copy_buff, buf_, writer_index_);
    delete(buf_);
    buf_ = copy_buff;
    capacity_ = new_capacity;
}

size_t ByteBuffer::WriteableBytes() const
{
    return capacity_ - writer_index_;
}

void ByteBuffer::SetByte(size_t index, unsigned char byte)
{
    if (index > ReadableBytes())
    {
        EnsureWritableBytes(index - ReadableBytes());
    }
    size_t index_to_write = reader_index_ + index;
    if (index_to_write >= writer_index_)
    {
        memset(buf_ + writer_index_, 0, index_to_write - writer_index_);
        writer_index_ = index_to_write + 1;
    }
    buf_[index_to_write] = byte;
}

const bool ByteBuffer::operator==(const ByteBuffer& other) const
{
    if (ReadableBytes() != other.ReadableBytes())
    {
        return false;
    }
    for (size_t i = 0; i < ReadableBytes(); i++)
    {
        if (buf_[reader_index_ + i] != other.buf_[other.reader_index_ + i])
        {
            return false;
        }
    }
    return true;
}

const bool ByteBuffer::operator!=(const ByteBuffer& other) const
{
    return !(*this == other);
}

int64_t ByteBuffer::ReadLong()
{
    unsigned char bytes[8];
    ReadBytes(bytes, 8);
    uint64_t value =
            ((uint64_t) bytes[0] & 0xff) << 56 |
            ((uint64_t) bytes[1] & 0xff) << 48 |
            ((uint64_t) bytes[2] & 0xff) << 40 |
            ((uint64_t) bytes[3] & 0xff) << 32 |
            ((uint64_t) bytes[4] & 0xff) << 24 |
            ((uint64_t) bytes[5] & 0xff) << 16 |
            ((uint64_t) bytes[6] & 0xff) << 8 |
            ((uint64_t) bytes[7] & 0xff);
    return *(int64_t*)&value;
}

float ByteBuffer::ReadFloat()
{
    unsigned char bytes[4];
    ReadBytes(bytes, 4);
    uint32_t value =
            ((uint32_t) bytes[0] & 0xff) << 24 |
            ((uint32_t) bytes[1] & 0xff) << 16 |
            ((uint32_t) bytes[2] & 0xff) << 8 |
            ((uint32_t) bytes[3] & 0xff);
    return *(float*)&value;
}

int16_t ByteBuffer::ReadShort()
{
    unsigned char bytes[2];
    ReadBytes(bytes, 2);
    uint16_t value =
            ((uint16_t) bytes[0] & (uint16_t) 0xff) << 8 |
            ((uint16_t) bytes[1] & (uint16_t) 0xff);
    return *(int16_t*)&value;
}

int32_t ByteBuffer::ReadMedium()
{
    unsigned char bytes[3];
    ReadBytes(bytes, 3);
    uint32_t value =
            ((uint32_t) bytes[0] & 0xff) << 16 |
            ((uint32_t) bytes[1] & 0xff) << 8 |
            ((uint32_t) bytes[2] & 0xff);
    if ((value & 0x800000) != 0)
    {
        value |= 0xff000000;
    }
    return *(int32_t*)&value;
}

double ByteBuffer::ReadDouble()
{
    unsigned char bytes[8];
    ReadBytes(bytes, 8);
    uint64_t value =
            ((uint64_t) bytes[0] & 0xff) << 56 |
            ((uint64_t) bytes[1] & 0xff) << 48 |
            ((uint64_t) bytes[2] & 0xff) << 40 |
            ((uint64_t) bytes[3] & 0xff) << 32 |
            ((uint64_t) bytes[4] & 0xff) << 24 |
            ((uint64_t) bytes[5] & 0xff) << 16 |
            ((uint64_t) bytes[6] & 0xff) << 8 |
            ((uint64_t) bytes[7] & 0xff);
    return *(double*)&value;
}

size_t ByteBuffer::WriteLong(const int64_t value)
{
    uint64_t i = *(uint64_t*)&value;
    unsigned char bytes[8];
    bytes[0] = (unsigned char) (i >> 56);
    bytes[1] = (unsigned char) (i >> 48);
    bytes[2] = (unsigned char) (i >> 40);
    bytes[3] = (unsigned char) (i >> 32);
    bytes[4] = (unsigned char) (i >> 24);
    bytes[5] = (unsigned char) (i >> 16);
    bytes[6] = (unsigned char) (i >> 8);
    bytes[7] = (unsigned char) i;
    return WriteBytes(bytes, 8);
}

size_t ByteBuffer::WriteFloat(const float value)
{
    uint32_t i = *(uint32_t*)&value;
    unsigned char bytes[4];
    bytes[0] = (unsigned char) (i >> 24);
    bytes[1] = (unsigned char) (i >> 16);
    bytes[2] = (unsigned char) (i >> 8);
    bytes[3] = (unsigned char) i;
    return WriteBytes(bytes, 4);
}

size_t ByteBuffer::WriteShort(const int16_t value)
{
    uint16_t i = *(uint16_t*)&value;
    unsigned char bytes[2];
    bytes[0] = (unsigned char) (i >> 8);
    bytes[1] = (unsigned char) i;
    return WriteBytes(bytes, 2);
}

size_t ByteBuffer::WriteMedium(const int32_t value)
{
    uint32_t i = *(uint32_t*)&value;
    unsigned char bytes[3];
    bytes[0] = (unsigned char) (i >> 16);
    bytes[1] = (unsigned char) (i >> 8);
    bytes[2] = (unsigned char) i;
    return WriteBytes(bytes, 3);
}

size_t ByteBuffer::WriteDouble(const double value)
{
    uint64_t i = *(uint64_t*)&value;
    unsigned char bytes[8];
    bytes[0] = (unsigned char) (i >> 56);
    bytes[1] = (unsigned char) (i >> 48);
    bytes[2] = (unsigned char) (i >> 40);
    bytes[3] = (unsigned char) (i >> 32);
    bytes[4] = (unsigned char) (i >> 24);
    bytes[5] = (unsigned char) (i >> 16);
    bytes[6] = (unsigned char) (i >> 8);
    bytes[7] = (unsigned char) i;
    return WriteBytes(bytes, 8);
}
} // namespace bidfx_public_api::tools