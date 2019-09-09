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
#include <ios>
#include "string_char_iterator.h"

namespace  bidfx_public_api::tools
{

StringCharIterator::StringCharIterator(std::string string, const char delimiter)
{
    string_ = string;
    delimiter_ = delimiter;
    end_ = string.length();
    Reset();
}

void StringCharIterator::Reset(std::string string)
{
    string_ = string;
    end_ = string.length();
    Reset();
}

void StringCharIterator::Reset()
{
    position_ = end_ == 0 ? 1 : 0;
}

std::string StringCharIterator::RemainingSubstring()
{
    return HasNext() ? string_.substr(position_) : "";
}

std::string StringCharIterator::Next(const char delimiter)
{
    delimiter_ = delimiter;
    return Next();
}

std::string StringCharIterator::Next()
{
    if (!HasNext())
    {
        throw std::ios_base::failure("NoSuchElementException()");
    }

    size_t next = string_.find(delimiter_, position_);
    if (next == -1)
    {
        next = end_;
    }

    std::string token = string_.substr(position_, next - position_);
    position_ = next + 1;
    return token;
}

void StringCharIterator::SetDelimiter(const char delimiter)
{
    delimiter_ = delimiter;
}

size_t StringCharIterator::GetPosition()
{
    return position_;
}

int StringCharIterator::CountTokens()
{
    int count = 0;
    while (HasNext())
    {
        Next();
        ++count;
    }
    Reset();
    return count;
}

bool StringCharIterator::HasNext()
{
    return position_ <= end_;
}


}