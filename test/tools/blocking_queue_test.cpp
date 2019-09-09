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

#include <memory>
#include <thread>
#include "src/tools/blocking_queue.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::tools
{


TEST(BlockingQueueTest, test_can_push_then_pop)
{
    BlockingQueue<int> queue = BlockingQueue<int>();
    queue.Push(1);
    EXPECT_EQ(1, *queue.Pop());
}

TEST(BlockingQueueTest, test_can_push_three_times_and_pop_out_as_queue)
{
    BlockingQueue<int> queue = BlockingQueue<int>();
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);
    queue.Push(4);
    queue.Push(5);

    EXPECT_EQ(1, *queue.Pop());
    EXPECT_EQ(2, *queue.Pop());
    EXPECT_EQ(3, *queue.Pop());
    EXPECT_EQ(4, *queue.Pop());
    EXPECT_EQ(5, *queue.Pop());
}


TEST(BlockingQueueTest, test_empty_queue_pop_returns_end_of_queue)
{
    BlockingQueue<int> queue = BlockingQueue<int>();
    EXPECT_TRUE(queue.Pop() == queue.End());
}

TEST(BlockingQueueTest, test_not_empty_queue_does_not_pop_with_end_of_queue)
{
    BlockingQueue<int> queue = BlockingQueue<int>();
    queue.Push(1);
    EXPECT_FALSE(queue.Pop() == queue.End());
}

TEST(BlockingQueueTest, test_poll_will_timeout)
{
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

    BlockingQueue<int> queue = BlockingQueue<int>();
    queue.Poll(std::chrono::milliseconds(250));

    std::chrono::high_resolution_clock::time_point time_now = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration elapsed = time_now - start_time;

    EXPECT_GT(elapsed, std::chrono::milliseconds(250));
    EXPECT_TRUE(queue.Pop() == queue.End());
}

void PushToIntBlockingQueueAfterSleep(BlockingQueue<int>* queue, int element, std::chrono::milliseconds interval)
{
    std::this_thread::sleep_for(interval);
    queue->Push(element);
}

TEST(BlockingQueueTest, test_poll_will_block_until_second_thread_pushes_to_empty_queue)
{
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    BlockingQueue<int> queue = BlockingQueue<int>();

    std::thread thread(PushToIntBlockingQueueAfterSleep, &queue, 1, std::chrono::milliseconds(500));
    EXPECT_TRUE(*queue.Poll(std::chrono::milliseconds(1000)) == 1);

    std::chrono::high_resolution_clock::time_point time_now = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::duration elapsed = time_now - start_time;
    EXPECT_LT(elapsed, std::chrono::milliseconds(600));

    thread.join();
}

}