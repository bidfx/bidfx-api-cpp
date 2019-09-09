#include <chrono>
#include <src/tools/countdown_latch.h>

namespace bidfx_public_api::tools
{

CountdownLatch::CountdownLatch(uint32_t count)
{
    this->count = count;
}

bool CountdownLatch::Await(std::chrono::milliseconds duration)
{
    std::unique_lock<std::mutex> lck(lock);
    if (0 == count)
    {
        return true;
    }
    if (duration > std::chrono::milliseconds::zero())
    {
        cv.wait_for(lck, duration);
    }
    return count == 0;
}

void CountdownLatch::Await()
{
    std::unique_lock<std::mutex> lck(lock);
    if (0 == count)
    {
        return;
    }
    cv.wait(lck);
}

uint32_t CountdownLatch::GetCount()
{
    std::unique_lock<std::mutex> lck(lock);
    return count;
}

void CountdownLatch::CountDown()
{
    std::unique_lock<std::mutex> lck(lock);
    if (0 == count)
    {
        cv.notify_all();
        return;
    }
    --count;
    if (0 == count)
    {
        cv.notify_all();
    }
}

}
