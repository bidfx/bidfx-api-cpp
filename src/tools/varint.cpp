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

#include <string>
#include <stdexcept>
#include "varint.h"

namespace bidfx_public_api::tools
{

const std::string Varint::kNullString = std::string("\000", 1);

uint32_t Varint::ReadU32(InputStream& stream)
{
    unsigned int result = 0;
    for (int offset = 0; offset < 32; offset += kBitsPerByte)
    {
        unsigned char next_byte = stream.ReadByte();
        result |= (unsigned int) (next_byte & kMaskLow7Bits) << offset;  //Added MaskedLow7Bits TEST this
        if (IsFinalByte(next_byte))
        {
            break;
        }
    }
    return result;
}

uint64_t Varint::ReadU64(InputStream& stream)
{
    unsigned long long result = 0L;
    for (int offset = 0; offset < 64; offset += kBitsPerByte)
    {
        unsigned char next_byte = stream.ReadByte();
        result |= (unsigned long long) (next_byte & kMaskLow7Bits) << offset;
        if (IsFinalByte(next_byte))
        {
            break;
        }
    }
    return result;
}

void Varint::WriteU32(OutputStream& stream, uint32_t value)
{
    while ((value & ~kMaskLow7Bits) != 0)
    {
        stream.WriteByte((uint8_t) ((value & kMaskLow7Bits) | kContinuationBit));
        value = value >> kBitsPerByte;
    }

    stream.WriteByte((uint8_t) value);
}

void Varint::WriteU64(OutputStream& stream, uint64_t value)
{
    while ((value & ~kMaskLow7Bits) != 0)
    {
        stream.WriteByte((uint8_t) ((value & kMaskLow7Bits) | kContinuationBit));
        value = value >> kBitsPerByte;
    }

    stream.WriteByte((uint8_t) value);
}

bool Varint::IsFinalByte(int b)
{
    return (b & (1 << 7)) == 0;
}

std::string Varint::ReadString(InputStream& stream)
{
    std::string s;
    unsigned int length = Varint::ReadU32(stream);
    if (length == 0)
    {
        return kNullString;
    }

    for (int i = 0; i < length - 1; i++)
    {
        int nextByte = stream.ReadByte();
        if (nextByte == -1)
        {
            throw std::overflow_error("Stream ended while reading string");
        }
        s += (char) nextByte;
    }
    return s;
}

void Varint::WriteString(OutputStream& stream, std::string s)
{
    if (s == kNullString)
    {
        stream.WriteByte('\000');
        return;
    }

    uint8_t *bytes = (uint8_t *) s.c_str();
    uint32_t length = static_cast<uint32_t>(s.length());
    WriteU32(stream, length + 1);
    stream.WriteBytes(bytes, length);
}

std::vector<std::string> Varint::ReadStringVector(InputStream& stream)
{
    uint32_t length = ReadU32(stream);
    std::vector<std::string> string_vec;

    for (uint32_t i = 0; i < length; i++)
    {
        string_vec.push_back(std::move(ReadString(stream)));
    }

    return string_vec;
}

void Varint::WriteStringVector(OutputStream& stream, const std::vector<std::string> &arr)
{
    WriteU32(stream, (unsigned int) arr.size());
    for (int i = 0; i < arr.size(); i++)
    {
        WriteString(stream, arr[i]);
    }
}

long Varint::ZigzagToLong(unsigned long long zigzag)
{
    return (long) (zigzag >> 1) ^ -(long) (zigzag & 1L);
}

int Varint::ZigzagToInt(unsigned int zigzag)
{
    return (int) (zigzag >> 1) ^ -(int) (zigzag & 1);
}

} // namespace bidfx_public_api::tools
