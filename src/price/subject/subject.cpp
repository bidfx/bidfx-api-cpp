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
#include "include/price/subject/subject.h"
#include "include/price/subject/subject_utils.h"
#include "subject_key.h"
#include "subject_formatter.h"

namespace bidfx_public_api::price::subject
{
Subject::Subject()
{

}

Subject::Subject(std::vector<std::string> components)
{
    components_ = components;
}

Subject::Subject(const std::string& formatted_string) : Subject(SubjectUtils::CreateSubjectFromString(formatted_string))
{
}

Subject::Subject(const Subject& to_copy)
{
    components_ = to_copy.components_;
}

Subject::Subject(Subject&& to_move)
{
    components_ = std::move(to_move.components_);
}

Subject& Subject::operator=(const Subject& to_copy)
{
    components_ = to_copy.components_;
    return *this;
}

Subject& Subject::operator=(Subject&& to_move)
{
    components_ = std::move(to_move.components_);
    return *this;
}

std::string Subject::GetComponent(std::string key) const
{
    int index = SubjectUtils::BinarySearch(&components_, key);

    if (index >= 0)
    {
        return components_[index + 1];
    }

    return "";
}

const std::vector<std::string>& Subject::GetComponents() const
{
    return components_;
}

std::string Subject::ToString() const
{

    return SubjectFormatter().FormatSubjectComponents(components_);
}

int Subject::Compare(const Subject& that) const
{
    if (this == &that)
    {
        return 0;
    }

    size_t size = std::min(components_.size(), that.components_.size());

    for (size_t i = 0; i < size; i ++)
    {
        int str_compare = components_[i].compare(that.components_[i]);

        if (str_compare != 0)
        {
            return str_compare;
        }
    }

    if (components_.size() < that.components_.size())
    {
        return -1;
    }
    else if (components_.size() > that.components_.size())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool Subject::operator==(const Subject& that) const
{
    if (components_.size() != that.components_.size())
    {
        return false;
    }

    return Compare(that) == 0;
}

bool Subject::operator<(const Subject& that) const
{
    return Compare(that) < 0;
}

std::ostream& operator<<(std::ostream& os, const Subject& s)
{
    return os << s.ToString();
}

}