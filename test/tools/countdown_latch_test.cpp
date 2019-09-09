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
#include <future>
#include "src/tools/countdown_latch.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::tools
{

using bidfx_public_api::tools::CountdownLatch;



TEST(CountdownLatchTest, TestCountdownLatchCounter)
{
    auto latch = new CountdownLatch(5);
    ASSERT_EQ(5, latch->GetCount());
    latch->CountDown();
    ASSERT_EQ(4, latch->GetCount());
    latch->CountDown();
    ASSERT_EQ(3, latch->GetCount());
    latch->CountDown();
    ASSERT_EQ(2, latch->GetCount());
    latch->CountDown();
    ASSERT_EQ(1, latch->GetCount());
    latch->CountDown();
    ASSERT_EQ(0, latch->GetCount());
    latch->CountDown();
    ASSERT_EQ(0, latch->GetCount());
}

void SleepThenCountdownLatch(std::shared_ptr<CountdownLatch> latch, std::chrono::milliseconds interval, int times)
{
    for (; times > 0; --times)
    {
        std::this_thread::sleep_for(interval);
        latch->CountDown();
    }
}
TEST(CountdownLatchTest, TestCountdownLatchThreadLock)
{
    std::shared_ptr<CountdownLatch> latch = std::make_unique<CountdownLatch>(2);
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    std::thread thread(SleepThenCountdownLatch, latch, std::chrono::seconds(1), 2);
    bool result = latch->Await(std::chrono::seconds(30));
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    EXPECT_TRUE(result);
    EXPECT_EQ(0, latch->GetCount());
    EXPECT_LT(end.time_since_epoch() - start.time_since_epoch(),std::chrono::seconds(30));
    thread.join();
}

TEST(CountdownLatchTest, TestCountdownLatchThreadLockCountdownNotFinished)
{
    std::shared_ptr<CountdownLatch> latch = std::make_unique<CountdownLatch>(3);
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    std::thread thread(SleepThenCountdownLatch, latch, std::chrono::seconds(1), 2);
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    bool result = latch->Await(std::chrono::seconds(4));
    EXPECT_FALSE(result);
    EXPECT_EQ(1, latch->GetCount());
    EXPECT_GT(std::chrono::seconds(4), end.time_since_epoch() - start.time_since_epoch());
    thread.join();
}

TEST(CountdownLatchTest, TestCountdownLatchFromZeroIndefiniteAwait)
{
    CountdownLatch latch = CountdownLatch(0);
    auto asyncFuture = std::async(
            std::launch::async, [&latch]() {
                latch.Await();
            });
    EXPECT_TRUE(asyncFuture.wait_for(std::chrono::milliseconds(5000)) != std::future_status::timeout);
}


TEST(CountdownLatchTest, TestCountdownLatchFromZeroDefiniteAwait)
{
    CountdownLatch latch = CountdownLatch(0);
    std::chrono::time_point<std::chrono::system_clock> start  = std::chrono::system_clock::now();
    EXPECT_TRUE(latch.Await(std::chrono::milliseconds(10000)));
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    EXPECT_LT(end.time_since_epoch() - start.time_since_epoch(), std::chrono::milliseconds(10000));
}

TEST(CountdownLatchTest, TestCountLatchZeroCountZeroDuration)
{
    CountdownLatch latch = CountdownLatch(0);
    EXPECT_TRUE(latch.Await(std::chrono::milliseconds(0)));
}
}
