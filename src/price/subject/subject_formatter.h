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

#ifndef PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_FORMATTER_H_
#define PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_FORMATTER_H_

#include <string>
#include <vector>
#include <map>
#include "include/price/subject/subject_builder.h"

namespace bidfx_public_api::price::subject
{

class SubjectBuilder;

/**
 * @author Frazer Bennett Wilford
 */
class SubjectFormatter
{
private:
    class NumericCharacterEntity
    {
    private:
        std::map<char, std::string> encodings_table_;

        void AddCharacterEncoding(char c);
        std::string CharacterEntity(char c);
        std::string EncodeRemaining(std::string s, size_t start, const std::string &encoded);
        std::string DecodeRemaining(std::string s, size_t start);
        size_t NextDecodedChar(std::stringstream& ss, std::string s, size_t start);

    public:
        /**
         * Creates a new instance that encoded only &amp; (the ampersand character must always be encoded).
         */
        NumericCharacterEntity(std::vector<char> encodings);

        /**
         * Encodes a string as escaped HTML text.
         * @param s the text to encode.
         */
        std::string EncodeString(std::string s);

        /**
         * Decodes an HTML string with escape entities into a normal string.
         * @param s the text to decode.
         */
        std::string DecodeString(std::string s);
    };

    NumericCharacterEntity numeric_character_entity_;

public:
    static const char COMPONENT_SEPARATOR; //< Contains the separator between two subject components
    static const char KEY_VALUE_SEPARATOR; //< Contains the separator between the key and value of each subject component

    /**
     * Constructs a new SubjectFormatter using the default separator characters which are ',' for separating components and '=' for
     * separating key-value pairs.
     */
    SubjectFormatter();

    /**
     * Builds a formatted string of the key/values of each component
     * @param s the text to encode.
     * @return the formatted subject string
     */
    std::string FormatSubjectComponents(const std::vector<std::string>& components);

    /**
     * Configures a SubjectBuilder with the components inside the provided formatted subject string
     * @param formatted_subject the formatted string of subject components
     * @param builder the SubjectBuilder to setup
     */
    void ParseSubject(const std::string& formatted_subject, SubjectBuilder& builder);

};

}

#endif //PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_FORMATTER_H_
