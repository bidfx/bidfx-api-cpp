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
#include <vector>
#include <sstream>
#include "include/price/subject/subject_builder.h"
#include "include/price/subject/subject_utils.h"
#include "subject_formatter.h"

namespace bidfx_public_api::price::subject
{

SubjectBuilder::SubjectBuilder()
{
}

Subject SubjectBuilder::CreateSubject()
{
    return Subject(GetComponents());
}

SubjectBuilder& SubjectBuilder::SetComponent(const std::string& key, const std::string& value)
{
    AddComponent(key, value);
    return *this;
}

void SubjectBuilder::AddComponent(const std::string& key, const std::string& value)
{
    if (IsKeyInOrder(key))
    {
        AddComponentAt(components_.size(), key, value);
    }
    else
    {
        int index = SubjectUtils::BinarySearch(&components_, key);

        if (index < 0)
        {
            AddComponentAt(-1 - index, key, value);
        }
        else
        {
            components_[index + 1] = value;
        }
    }
}

void SubjectBuilder::AddComponentAt(int index, const std::string& key, const std::string& value)
{
    components_.insert(components_.begin() + index, key);
    components_.insert(components_.begin() + index + 1, value);
}

std::vector<std::string> SubjectBuilder::GetComponents()
{
    std::vector<std::string> components_copy = std::vector<std::string>();

    for (const std::string &component : components_)
    {
        components_copy.push_back(component);
    }

    return components_copy;
}

std::string SubjectBuilder::GetComponent(const std::string& key)
{
    for (unsigned long i = 0; i < components_.size(); i+= 2)
    {
        if ((components_.at(i)) == key)
        {
            return components_.at(i + 1);
        }
    }

    return "";
}

bool SubjectBuilder::IsKeyInOrder(std::string key)
{
    return components_.size() == 0 || components_[components_.size() - 2].compare(key) < 0;
}

bool SubjectBuilder::ContainsKey(const std::string &key)
{
    return SubjectUtils::BinarySearch(&components_, key) >= 0;
}

std::string SubjectBuilder::ToString()
{
    return subject::SubjectFormatter().FormatSubjectComponents(components_);
}


}