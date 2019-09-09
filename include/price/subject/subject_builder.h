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

#ifndef PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_BUILDER_H_
#define PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_BUILDER_H_

#include <string>
#include <memory>
#include "include/price/subject/subject.h"

namespace bidfx_public_api::price::subject
{

class Subject;

/**
 * This class provides a mutable subject builder.
 * @author Frazer Bennett Wilford
 */
class SubjectBuilder
{

private:
    std::vector<std::string> components_;

    void AddComponent(const std::string& key, const std::string& value);
    void AddComponentAt(int index, const std::string& key, const std::string& value);

    bool IsKeyInOrder(std::string key);

public:
    SubjectBuilder();

    /**
     * Creates a new subject from the components that has been set.
     */
    Subject CreateSubject();

    /**
     * Sets a component of the subject.
     * @param key the component's key (alphanumeric).
     * @param value the component's value (any visible ASCII string).
     * @return the builder so that calls can be chained.
     * @throws InvalidSubjectException if the key or value are invalid.
     */
    SubjectBuilder& SetComponent(const std::string& key, const std::string& value);

    /**
     * Creates and returns a deep copy of the subject components
     * @return contains a string for each key, followed by their subsequent value, etc
     */
    std::vector<std::string> GetComponents();

    /**
     * Gets the value of a subject component by name.
     * @param key the component key.
     * @return the value or empty string.
     */
    std::string GetComponent(const std::string& key);

    /**
     * Tests if the subject contains a component with a given key.
     * @param key the component key.
     * @return the true if the key is present and false otherwise.
     */
    bool ContainsKey(const std::string& key);

    std::string ToString();
};

}

#endif //PUBLIC_API_CPP_PRICE_SUBJECT_SUBJECT_BUILDER_H_
