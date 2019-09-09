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

#ifndef PUBLIC_API_CPP_TOOLS_STRING_CHAR_ITERATOR_H_
#define PUBLIC_API_CPP_TOOLS_STRING_CHAR_ITERATOR_H_

#include <string>

namespace bidfx_public_api::tools
{

/**
 * A simple string iterator that uses a single character as a delimiter. Compared to StringTokenizer, it returns an empty String
 * between consecutive delimiters; making it useful for jobs such as comma or tab separated file parsing. Its should also be a bit
 * faster than StringTokenizer.
 * @author Frazer Bennett Wilford
 */
class StringCharIterator
{
private:
    std::string string_;
    size_t position_;
    size_t end_;
    char delimiter_;

public:
    /**
     * Construct a new SimpleStringTokenizer.
     * @param string the string to be tokenized.
     * @param delimiter the delimiter that splits the string.
     */
    StringCharIterator(std::string string, const char delimiter);

    /**
     * Resets the tokenizer with a new string.
     * @param string the next string to be tokenized.
     */
    void Reset(std::string string);

    /**
     * Resets the tokenizer to start again on the same string.
     */
    void Reset();

    /**
     * Gets the remaining, as yet untokenized portion of the tokenized string.
     */
    std::string RemainingSubstring();

    /**
     * Change the delimiter and returns the next token or an empty string if the end is reached.
     * @param delimiter the new delimiter character.
     */
    std::string Next(const char delimiter);

    std::string Next();

    /**
     * Sets the delimiter to use from now on.
     * @param delimiter the delimiter that splits the string.
     */
    void SetDelimiter(const char delimiter);

    size_t GetPosition();

    /**
     * Counts the remaining tokens and resets the tokenizer.
     */
    int CountTokens();

    bool HasNext();
};

} // namespace bidfx_public_api::tools


#endif //PUBLIC_API_CPP_TOOLS_STRING_CHAR_ITERATOR_H_
