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

#ifndef PUBLIC_API_CPP_PRICE_PROVIDER_SUBSCRIPTION_SYNC_H_
#define PUBLIC_API_CPP_PRICE_PROVIDER_SUBSCRIPTION_SYNC_H_

#include <map>
#include "include/price/subject/subject.h"
#include "pixie_message.h"
#include "control_operation.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::price::subject::Subject;

class SubscriptionSync : public PixieMessage
{
private:
    static const uint32_t COMPRESSION_LEVEL = 6;
    static const uint32_t COMPRESSION_BIT = 0;
    static const uint32_t CONTROLS_BIT = 1; // since version 2
    static const uint32_t UNCHANGED_BIT = 2; // since version 2

    uint32_t edition_;
    size_t size_;
    uint32_t options_ = 0;

    std::map<uint32_t, ControlOperationEnum> controls_;
    std::vector<Subject> subjects_;

public:
    SubscriptionSync(uint32_t edition, std::vector<Subject> subject_set);
    SubscriptionSync(const SubscriptionSync& to_copy);
    SubscriptionSync(const SubscriptionSync&& to_move);
    SubscriptionSync& operator=(const SubscriptionSync& to_copy);
    SubscriptionSync& operator=(const SubscriptionSync&& to_move);

    /**
     * Checks is the message is compressed.
     */
    bool IsCompressed() const;

    /**
     * Checks is the message is contains subject controls.
     * @since protocol version 2
     */
    bool HasControls() const;

    /**
     * Checks if the message includes a change to the subject set edition and therefore provides the new set of subjects.
     * @since protocol version 2
     */
    bool IsChangedEdition() const;

    /**
     * Sets if the message includes a change to the subject set edition and therefore provides the new set of subjects.
     * @since protocol version 2
     */
    void SetChangedEdition(bool changed);

    /**
     * Gets the size of the subscription set. This will be provided even if the subjects are not when the set is unchanged.
     * @since protocol version 2
     */
    size_t GetSize() const;

    /**
     * Gets the subject list.
     */
    const std::vector<Subject>& GetSubjects() const;

    /**
     * Adds a subscription control operation.
     * @param sid the subject ID.
     * @param controlOperation the operation to add.
     * @since protocol version 2
     */
    void AddControl(uint32_t sid, ControlOperationEnum control_operation);

    /**
     * Checks is the message is compressed.
     */
    void SetCompressed(bool compressed);

    /**
     * Gets the subscription set edition number.
     */
    uint32_t GetEdition() const;

     /**
      * Returns the controls for this subscription sync.
      */
     const std::map<uint32_t, ControlOperationEnum>& GetControls() const;


    ByteBuffer Encode(int version) const override;
    const std::string ToString() override;

    std::string Summarize();
};

}

#endif //PUBLIC_API_CPP_PRICE_PROVIDER_SUBSCRIPTION_SYNC_H_
