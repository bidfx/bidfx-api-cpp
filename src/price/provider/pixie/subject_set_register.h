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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_SUBJECT_SET_REGISTER_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_SUBJECT_SET_REGISTER_H_

#include <mutex>
#include <memory>
#include <deque>
#include "include/price/subject/subject.h"
#include "src/price/provider/pixie/messages/subscription_sync.h"
#include "src/price/provider/pixie/fields/field_def.h"
#include "src/tools/optional.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::subject::Subject;

class SubjectState
{
private:
    int state_;

public:
    SubjectState(int state);
    void SetState(int state);
    int GetState() const;
};

class EditionData
{
private:
    std::vector<Subject> subjects_;

public:
    EditionData();
    EditionData(std::vector<Subject> subjects);
    EditionData(const EditionData& to_copy);
    EditionData(EditionData&& to_move);
    EditionData& operator=(const EditionData& to_copy);
    EditionData& operator=(EditionData&& to_move);
    std::vector<Subject>& GetSubjects();
};

class SubjectSetRegister
{
private:
    static const int STATE_UNSUBSCRIBED = 0;
    static const int STATE_NEWLY_SUBSCRIBED = 1;
    static const int STATE_SUBSCRIBED = 2;
    static const int STATE_REFRESH = 3;
    static const int STATE_TOGGLE = 4;

    std::mutex mutex_;
    bool modified_ = false;
    std::map<Subject, SubjectState> subject_state_;
    std::map<int, std::shared_ptr<EditionData>> subject_set_cache_;

    std::vector<Subject> CurrentSubjectSetSorted();
    SubscriptionSync CreateSubscriptionSync(int edition, std::vector<Subject>& subject_set);
    void AddSubscriptionControls(SubscriptionSync& subscription_sync);
    void CleanUnsubscribeState();
    void ClearSubjectState(const Subject& subject);
    void RemoveEditionsOlderThan(const uint32_t edition);

public:
    SubjectSetRegister();

    /**
     * Registers a new subscription.
     * @param subject the subject being subscribed to.
     * @param refresh true if this is a refresh of an existing subscription.
     */
    void Register(Subject subject, bool refresh);

    /**
     * Unregisters a subject that is being unsubscribed from.
     * @param subject the subject being unsubscribed from.
     */
    void Unregister(const Subject& subject);

    /**
     * Creates the next subscriptionSync message to inform the server of the client's current view of its subscription set. A
     * subscriptionSync will be generated if the subscription set has changed or if there have been subscription control operations.
     * If nothing needs to be synchronised then null is returned.
     */
    std::optional<SubscriptionSync> NextSubscriptionSync();

    /**
     * This method returns the subject set for the specified subject set edition.
     * @param edition of the set to return
     * @return set for the specified edition or null if not found
     */
    std::optional<std::vector<Subject>> SubjectSetByEdition(uint32_t edition);

    /**
     * Tests if a subject is currently subscribed to.
     * @param subject the subject to test.
     */
    bool IsCurrentlySubscribed(const Subject& subject);

    /**
     * Retirmines if a given subject is a request for quote
     * @param subject
     * @return
     */
    bool IsRequestForQuote(const Subject& subject);

    /**
     * Completely resets this class so that it can be reused.
     */
    void Reset();

    // only for tests
    std::map<Subject, SubjectState>& GetSubjectState();
};


} // namespace bidfx_public_api::price::pixie

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_SUBJECT_SET_REGISTER_H_
