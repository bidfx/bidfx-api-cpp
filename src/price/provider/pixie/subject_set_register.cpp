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

#include <algorithm>
#include <set>
#include "src/price/subject/subject_key.h"
#include "src/price/subject/subject_comparator.h"
#include "include/price/subject/subject.h"
#include "subject_set_register.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::subject::SubjectComparator;
using bidfx_public_api::price::subject::SubjectKey;

SubjectSetRegister::SubjectSetRegister()
{
    subject_set_cache_.insert_or_assign(0, std::make_shared<EditionData>());
}

void SubjectSetRegister::Register(Subject subject, bool refresh)
{
    std::unique_lock<std::mutex> lck(mutex_);
    modified_ = true;
    auto state = subject_state_.find(subject);

    if (state == subject_state_.end())
    {
        subject_state_.emplace(std::move(subject), SubjectState(STATE_NEWLY_SUBSCRIBED));
    }
    else
    {
        if (state->second.GetState() == STATE_UNSUBSCRIBED)
        {
            if (IsRequestForQuote(subject))
            {
                state->second.SetState(STATE_TOGGLE);
            }
            else
            {
                state->second.SetState(refresh? STATE_REFRESH : STATE_SUBSCRIBED);
            }
        }
        else if (refresh && state->second.GetState() == STATE_SUBSCRIBED)
        {
            state->second.SetState(STATE_REFRESH);
        }
    }
}

std::optional<SubscriptionSync> SubjectSetRegister::NextSubscriptionSync()
{
    std::unique_lock<std::mutex> lck(mutex_);
    if (!modified_) { return {}; }
    modified_ = false;

    std::vector<Subject> subject_set = CurrentSubjectSetSorted();
    int edition = subject_set_cache_.empty() ? 0 : subject_set_cache_.rbegin()->first;
    auto edition_data = subject_set_cache_.find(edition);

    if (edition_data != subject_set_cache_.end() && subject_set == edition_data->second->GetSubjects())
    {
        SubscriptionSync subscription_sync = CreateSubscriptionSync(edition, subject_set);
        if (!subscription_sync.HasControls())
        {
            return {};
        }
        subscription_sync.SetChangedEdition(false);
        return std::move(subscription_sync);
    }

    subject_set_cache_.insert_or_assign(++edition, std::make_shared<EditionData>(subject_set));
    SubscriptionSync result = CreateSubscriptionSync(edition, subject_set);
    CleanUnsubscribeState();
    return std::move(result);
}

SubscriptionSync SubjectSetRegister::CreateSubscriptionSync(int edition, std::vector<Subject>& subject_set)
{
    SubscriptionSync subscription_sync(edition, subject_set);
    AddSubscriptionControls(subscription_sync);
    return std::move(subscription_sync);
}

void SubjectSetRegister::AddSubscriptionControls(SubscriptionSync& subscription_sync)
{
    uint32_t sid = 0;

    for (const Subject& subject : subscription_sync.GetSubjects())
    {
        SubjectState& state = subject_state_.find(subject)->second;
        if (state.GetState() == STATE_UNSUBSCRIBED)
        {
            ClearSubjectState(subject);
        }
        else if (state.GetState() == STATE_REFRESH)
        {
            subscription_sync.AddControl(sid, ControlOperationEnum::REFRESH);
        }
        else if (state.GetState() == STATE_TOGGLE)
        {
            subscription_sync.AddControl(sid, ControlOperationEnum::TOGGLE);
        }
        state.SetState(STATE_SUBSCRIBED);
        sid ++;
    }
}

std::vector<Subject> SubjectSetRegister::CurrentSubjectSetSorted()
{
    std::vector<Subject> subject_set;

    for (auto const& [subject, state] : subject_state_)
    {
        if (state.GetState() != STATE_UNSUBSCRIBED)
        {
            subject_set.push_back(Subject(subject));
        }
    }

    std::stable_sort(subject_set.begin(), subject_set.end(), SubjectComparator);
    return std::move(subject_set);
}

void SubjectSetRegister::CleanUnsubscribeState()
{
    std::map<Subject, SubjectState> subject_state_filtered_copy;

//    for (std::map<subject, std::shared_ptr<SubjectState>>::iterator itt = subject_state_.begin();
//         itt != subject_state_.end(); itt ++)
    for (auto const& [subject, state] : subject_state_)
    {
        if (state.GetState() != STATE_UNSUBSCRIBED)
        {
            subject_state_filtered_copy.insert_or_assign(subject, state);
        }
    }

    subject_state_ = std::move(subject_state_filtered_copy);
}

void SubjectSetRegister::Unregister(const Subject& subject_to_unregister)
{
    std::unique_lock<std::mutex> lck(mutex_);
    modified_ = true;

    auto state = subject_state_.find(subject_to_unregister);

    if (state == subject_state_.end() || state->second.GetState() == STATE_NEWLY_SUBSCRIBED)
    {
        ClearSubjectState(subject_to_unregister);
    }
    else
    {
        state->second.SetState(STATE_UNSUBSCRIBED);
    }
}

void SubjectSetRegister::ClearSubjectState(const Subject& subject_to_unregister)
{
    subject_state_.erase(subject_to_unregister);
}

bool SubjectSetRegister::IsRequestForQuote(const Subject& subject)
{
    std::string request_for = subject.GetComponent(SubjectKey::REQUEST_TYPE);
    return "Quote" == request_for;
}

std::map<Subject, SubjectState>& SubjectSetRegister::GetSubjectState()
{
    return subject_state_;
}

std::optional<std::vector<Subject>> SubjectSetRegister::SubjectSetByEdition(uint32_t edition)
{
    std::unique_lock<std::mutex> lck(mutex_);

    RemoveEditionsOlderThan(edition);

    auto edition_data = subject_set_cache_.find(edition);
    if (edition_data == subject_set_cache_.end())
    {
        return {};
    }

    return edition_data->second->GetSubjects();
}

void SubjectSetRegister::RemoveEditionsOlderThan(const uint32_t edition)
{
    std::set<int> editions_to_remove;
    for (const auto edition_data : subject_set_cache_)
    {
        if (edition_data.first < edition)
        {
            editions_to_remove.insert(edition_data.first);
        }
    }
    for (const int old_edition : editions_to_remove)
    {
        subject_set_cache_.erase(old_edition);
    }
}

bool SubjectSetRegister::IsCurrentlySubscribed(const Subject& subject_to_find)
{
    std::unique_lock<std::mutex> lck(mutex_);
    auto state = subject_state_.find(subject_to_find);
    return !(state == subject_state_.end() || state->second.GetState() == STATE_UNSUBSCRIBED);
}

void SubjectSetRegister::Reset()
{
    std::unique_lock<std::mutex> lck(mutex_);
    subject_state_.clear();
    subject_set_cache_.clear();
}

/*
 * SubjectState
 */

SubjectState::SubjectState(int state)
{
    state_ = state;
}

void SubjectState::SetState(int state)
{
    state_ = state;
}

int SubjectState::GetState() const
{
    return state_;
}

/*
 * Edition Data
 */

EditionData::EditionData()
{}

EditionData::EditionData(std::vector<Subject> subjects)
{
    subjects_ = subjects;
}

EditionData::EditionData(const EditionData& to_copy)
{
    subjects_ = to_copy.subjects_;
}

EditionData::EditionData(EditionData&& to_move)
{
    subjects_ = std::move(to_move.subjects_);
}

EditionData& EditionData::operator=(const EditionData& to_copy)
{
    subjects_ = to_copy.subjects_;
    return *this;
}

EditionData& EditionData::operator=(EditionData&& to_move)
{
    subjects_ = std::move(to_move.subjects_);
}

std::vector<Subject>& EditionData::GetSubjects()
{
    return subjects_;
}

}