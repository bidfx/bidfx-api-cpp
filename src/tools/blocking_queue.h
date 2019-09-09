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

#ifndef PUBLIC_API_CPP_TOOLS_BLOCKING_QUEUE_H_
#define PUBLIC_API_CPP_TOOLS_BLOCKING_QUEUE_H_

#include <mutex>
#include <condition_variable>
#include <deque>

namespace bidfx_public_api::tools
{

template<typename T>
class BlockingQueue
{
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    std::deque<T> queue_;

public:
    void Push(T& value)
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push_back(std::move(value));
        }

        condition_.notify_all();
    }

    void Push(T&& value)
    {
        Push(value);
    }

    typename std::deque<T>::iterator Poll(std::chrono::milliseconds duration)
    {
        {
            std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            std::unique_lock<std::mutex> lock(mutex_);
            std::chrono::nanoseconds elapsed = std::chrono::steady_clock::now() - start;
            while (queue_.empty() && elapsed < duration)
            {
                condition_.wait_for(lock, duration - elapsed);
                elapsed = std::chrono::steady_clock::now() - start;
            }
        }
        return Pop();
    }

    typename std::deque<T>::iterator Pop()
    {
        std::unique_lock<std::mutex> lock(mutex_);

        if (queue_.empty())
        {
            return queue_.end();
        }

        auto element = queue_.begin();
        queue_.pop_front();
        return element;
    }

    typename std::deque<T>::iterator End()
    {
        return queue_.end();
    }
};

}

#endif //PUBLIC_API_CPP_TOOLS_BLOCKING_QUEUE_H_
