#ifndef __COUNTDOWNLATCH_NIPUN__
#define __COUNTDOWNLATCH_NIPUN__

#include <inttypes.h>
#include <stdint.h>
#include <mutex>
#include <condition_variable>

namespace bidfx_public_api::tools
{
    class CountdownLatch {
    public:
        /*! Constructor
          \param count, the value the countdownlatch object should be initialized with
        */
        CountdownLatch(uint32_t count);

        /*!
            await causes the caller to wait until the latch is counted down to zero, upto a specified amount of time
          \param duration is the maximum amount of time to wait before returning
          \return true if the count reached zero and false if the waiting time elapsed before the count reached zero
        */
        bool Await(std::chrono::milliseconds duration);

        /*!
           Wait until the latch is counted down to zero, waiting forever if necessary
        */
        void Await();

        /*!
          Countdown decrements the count of the latch, signalling all waiting thread if the 
          count reaches zero.
         */
        void CountDown();

        /*!
          get_count returns the current count  
         */
        uint32_t GetCount();

    private:
        std::condition_variable cv;
        std::mutex lock;
        uint32_t count;
    };
}

#endif
