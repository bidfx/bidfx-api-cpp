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

#include <sstream>
#include "src/tools/string_char_iterator.h"
#include "src/price/subject/subject_formatter.h"

namespace bidfx_public_api::price::subject
{

using bidfx_public_api::tools::StringCharIterator;

const char SubjectFormatter::COMPONENT_SEPARATOR = ',';
const char SubjectFormatter::KEY_VALUE_SEPARATOR = '=';

SubjectFormatter::SubjectFormatter() : numeric_character_entity_ (SubjectFormatter::NumericCharacterEntity({SubjectFormatter::COMPONENT_SEPARATOR, ' '}))
{
}

std::string SubjectFormatter::FormatSubjectComponents(const std::vector<std::string>& components)
{
    if (components.empty())
    {
        return "";
    }

    NumericCharacterEntity entity = numeric_character_entity_;
    std::stringstream ss;

    ss << components.at(0) << KEY_VALUE_SEPARATOR << components.at(1);
    for (size_t i = 2; i < components.size(); i += 2)
    {
        ss << COMPONENT_SEPARATOR << components.at(i) << KEY_VALUE_SEPARATOR << entity.EncodeString(components.at(i + 1));
    }

    return ss.str();
}

void SubjectFormatter::ParseSubject(const std::string& formatted_subject, SubjectBuilder& builder)
{
    if (formatted_subject.empty())
    {
        throw std::runtime_error("empty subject string");
    }

    StringCharIterator iterator = StringCharIterator(formatted_subject, COMPONENT_SEPARATOR);

    try
    {
        while (iterator.HasNext())
        {
            std::string key = iterator.Next(KEY_VALUE_SEPARATOR);
            std::string encoded = iterator.Next(COMPONENT_SEPARATOR);
            std::string value = numeric_character_entity_.DecodeString(encoded);
            builder.SetComponent(key, value);
        }
    }
    catch (std::runtime_error &e)
    {
        std::stringstream ss;
        ss << "subject parse error: " << e.what();
        throw std::runtime_error(ss.str());
    }
}

/*****************************
 *  Numeric Character Entity
 */

SubjectFormatter::NumericCharacterEntity::NumericCharacterEntity(std::vector<char> encodings)
{
    AddCharacterEncoding('&');

    for (char c : encodings)
    {
        AddCharacterEncoding(c);
    }
}

void SubjectFormatter::NumericCharacterEntity::AddCharacterEncoding(char c)
{
    encodings_table_[c] = CharacterEntity(c);
}

std::string SubjectFormatter::NumericCharacterEntity::CharacterEntity(char c)
{
    std::stringstream ss;
    ss << "&#" << (int) c << ';';
    return ss.str();
}

std::string SubjectFormatter::NumericCharacterEntity::EncodeString(std::string s)
{
    if (s.empty())
    {
        return "";
    }

    for (size_t i = 0; i < s.length(); i++)
    {
        char c = s.at(i);
        std::string encoded = c < 255 ? encodings_table_[c] : CharacterEntity(c);
        if (!encoded.empty())
        {
            return EncodeRemaining(s, i, encoded);
        }
    }

    return s;
}

std::string SubjectFormatter::NumericCharacterEntity::EncodeRemaining(std::string s, size_t start, const std::string& encoded)
{
    std::stringstream ss;
    ss << s.substr(0, start);
    ss << encoded;

    std::string next_encoded;

    for (size_t i = start + 1; i < s.length(); i++)
    {
        char c = s.at(i);
        next_encoded = c < 255 ? encodings_table_[c] : CharacterEntity(c);

        if (!next_encoded.empty())
        {
            ss << c;
        }
        else
        {
            ss << next_encoded;
        }
    }

    return ss.str();
}

std::string SubjectFormatter::NumericCharacterEntity::DecodeString(std::string s)
{
    if (s.empty())
    {
        return "";
    }

    size_t length = s.length();

    for (size_t i = 0; i < length; i++)
    {
        char c = s.at(i);

        if (c == '&' && i < length - 3)
        {
            return DecodeRemaining(s, i);
        }
    }

    return s;
}

std::string SubjectFormatter::NumericCharacterEntity::DecodeRemaining(std::string s, size_t start)
{
    if (s.empty())
    {
        return "";
    }

    std::stringstream ss;
    ss << s.substr(0, start);

    for (size_t i = start + 1; i < s.length(); i++)
    {
        char c = s.at(i);

        if (c == '&' && i < s.length() - 3)
        {
            i = NextDecodedChar(ss, s, i);
        }
        else
        {
            ss << c;
        }
    }

    return ss.str();
}

size_t SubjectFormatter::NumericCharacterEntity::NextDecodedChar(std::stringstream &ss, std::string s, size_t start)
{
    size_t i = start + 1;
    char c = s.at(i++);

    if (c == '#')
    {
        c = s.at(i++);

        if (c >= '0' && c <= '9')
        {
            int code = c - '0';

            for (c = s.at(i++); c <= '9'; c = s.at(i++))
            {
                if (i == s.length()) { break; }

                code *= 10;
                code += c - '0';
            }

            if (c == ';')
            {
                ss << (char) code;
                return i - 1;
            }
        }
    }

    ss << '&';
    return start;
}

}
