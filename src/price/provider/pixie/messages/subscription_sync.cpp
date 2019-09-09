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
#include "src/tools/varint.h"
#include "src/price/provider/pixie/pixie_version.h"
#include "src/tools/uncompressed_buffer_compressor.h"
#include "src/tools/zlib_buffer_compressor.h"
#include "subscription_sync.h"
#include "pixie_message_type.h"

namespace bidfx_public_api::price::pixie
{

using bidfx_public_api::tools::Varint;
using bidfx_public_api::tools::BufferCompressor;
using bidfx_public_api::tools::UncompressedBufferCompressor;
using bidfx_public_api::tools::ZLIBBufferCompressor;

SubscriptionSync::SubscriptionSync(uint32_t edition, std::vector<Subject> subject_set)
{
    if (edition < 0)
    {
        std::stringstream ss;
        ss << "edition may not be negative: " << edition;
        throw std::invalid_argument(ss.str());
    }

    edition_ = edition;
    subjects_ = subject_set;
    size_ = subjects_.size();
}

SubscriptionSync::SubscriptionSync(const SubscriptionSync& to_copy)
{
    edition_ = to_copy.edition_;
    subjects_ = to_copy.subjects_;
    size_ = to_copy.size_;
    controls_ = to_copy.controls_;
    options_ = to_copy.options_;
}

SubscriptionSync::SubscriptionSync(const SubscriptionSync&& to_move)
{
    edition_ = to_move.edition_;
    subjects_ = std::move(to_move.subjects_);
    size_ = to_move.size_;
    controls_ = std::move(to_move.controls_);
    options_ = to_move.options_;
}

SubscriptionSync& SubscriptionSync::operator=(const SubscriptionSync& to_copy)
{
    edition_ = to_copy.edition_;
    subjects_ = to_copy.subjects_;
    size_ = to_copy.size_;
    controls_ = to_copy.controls_;
    options_ = to_copy.options_;
    return *this;
}

SubscriptionSync& SubscriptionSync::operator=(const SubscriptionSync&& to_move)
{
    edition_ = to_move.edition_;
    subjects_ = std::move(to_move.subjects_);
    size_ = to_move.size_;
    controls_ = std::move(to_move.controls_);
    options_ = to_move.options_;
    return *this;
}

bool SubscriptionSync::IsCompressed() const
{
    return (options_ & (1 << COMPRESSION_BIT)) != 0;
}

bool SubscriptionSync::HasControls() const
{
    return (options_ & (1 << CONTROLS_BIT)) != 0;
}

size_t SubscriptionSync::GetSize() const
{
    return size_;
}

bool SubscriptionSync::IsChangedEdition() const
{
    return (options_ & (1 << UNCHANGED_BIT)) == 0;
}

void SubscriptionSync::SetChangedEdition(bool changed)
{
    bool on = !changed;
    options_ = (on ? (options_ | (1 << UNCHANGED_BIT)) : (options_ & ~(1 << UNCHANGED_BIT)));
}

void SubscriptionSync::AddControl(uint32_t sid, ControlOperationEnum control_operation)
{
    if (sid < 0 || sid >= size_)
    {
        std::stringstream error_message;
        error_message << "control SID (" << sid << ") not in range 0..<" << size_;
        throw std::invalid_argument(error_message.str());
    }

    controls_.insert_or_assign(sid, control_operation);
    options_ = (options_ | (1 << CONTROLS_BIT));
}

void SubscriptionSync::SetCompressed(bool compressed)
{
    options_ = (compressed ? (options_ | (1 << COMPRESSION_BIT)) : (options_ & ~(1 << COMPRESSION_BIT)));
}

const std::vector<Subject>& SubscriptionSync::GetSubjects() const
{
    return subjects_;
}

uint32_t SubscriptionSync::GetEdition() const
{
    return edition_;
}

const std::map<uint32_t, ControlOperationEnum>& SubscriptionSync::GetControls() const
{
    return controls_;
}

ByteBuffer SubscriptionSync::Encode(int version) const
{
    ByteBuffer message_frame;
    message_frame.WriteByte(PixieMessageType::SUBSCRIPTION_SYNC);
    Varint::WriteU32(message_frame, (unsigned int) (version < PixieVersion::VERSION2 ? options_ & 1 : options_));
    Varint::WriteU32(message_frame, (unsigned int) edition_);
    Varint::WriteU32(message_frame, (unsigned int) size_);

    BufferCompressor* compressor = IsCompressed() ? (BufferCompressor*) new ZLIBBufferCompressor(5) : (BufferCompressor*) new UncompressedBufferCompressor();
    ByteBuffer subject_buffer;

    if (IsChangedEdition())
    {
        for (Subject subject : subjects_)
        {
            Varint::WriteStringVector(subject_buffer, subject.GetComponents());
            compressor->Compress(subject_buffer);
        }
    }
    if (HasControls() && version >= PixieVersion::VERSION2)
    {
        Varint::WriteU32(subject_buffer, static_cast<unsigned int>(controls_.size()));

        for (auto itt = controls_.begin(); itt != controls_.end(); itt ++)
        {
            Varint::WriteU32(subject_buffer, (unsigned int) itt->first);
            subject_buffer.WriteByte(ControlOperation::GetCode(itt->second));
        }
        compressor->Compress(subject_buffer);
    }

    message_frame.WriteBytes(compressor->GetCompressed());
    delete compressor;
    return std::move(message_frame);
}

const std::string SubscriptionSync::ToString()
{
    std::stringstream ss;
    ss << Summarize() << " [\n";

    if (subjects_.empty())
    {
        for (std::map<uint32_t, ControlOperationEnum>::iterator it = controls_.begin(); it != controls_.end(); it++)
        {
            int sid = it->first;
            ss << " SID=" << sid << " " << ControlOperation::GetCode(it->second) << "\n";
        }
    }
    else
    {
        for (size_t i = 0; i < subjects_.size(); i ++)
        {
            if (controls_.find(static_cast<const int &>(i)) == controls_.end())
            {
                ss << "  " << subjects_.at(i).ToString() << std::endl;
            }
            else
            {
                ss << "  " << subjects_.at(i).ToString() <<  " (" << ControlOperation::ToString(controls_[i]) << ")" << std::endl;
            }
        }
    }

    ss << "]";

    return ss.str();
}

std::string SubscriptionSync::Summarize()
{
    std::stringstream ss;
    ss << "SubscriptionSync(edition=" << edition_ <<
          ", compressed=";

    IsCompressed() ? ss << "true" : ss << "false";

    ss << ", controls=" << controls_.size() << ", changed=";

    IsChangedEdition() ? ss << "true" : ss << "false";

    ss << ", subjects=" << size_ << ")";

    return ss.str();
}

}