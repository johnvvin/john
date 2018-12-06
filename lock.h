#pragma once

#include <inttypes.h>
#include <stdint.h>
#include <mutex>
#include <condition_variable>

namespace mlock
{
// time in nanoseconds
// 1 Millisecond = 1,000,000 Nanoseconds
#define MSEC001 1000000ULL
// 10 Milliseconds = 10,000,000 Nanoseconds
#define MSEC010 MSEC001*10
// 100 Milliseconds = 100,000,000 Nanoseconds
#define MSEC100 MSEC010*10
// 1 second = 1,000,000,000 Nanoseconds
#define SEC001  MSEC100*10
// 10 seconds = 10,000,000,000 Nanoseconds
#define SEC010  SEC001*10
    class CLock {
    public:
        /*! Constructor
          \param count, the value the countdownlatch object should be initialized with
        */
        CLock(uint64_t count);

        /*!
            await causes the caller to wait until the latch is counted down to zero,
            if wait time nanosecs is not zero, then maximum wait is for nanosec nanoseconds
          \param nanosecs is waittime in nanoseconds, by default it is zero which specifies
           indefinite wait
        */
        void await(uint64_t nanosecs = 0);

        /*!
          Countdown decrements the count of the latch, signalling all waiting thread if the
          count reaches zero.
         */
        void count_down();

        /*!
          get_count returns the current count
         */
        uint64_t get_count();

        /*!
          set_count - set count value and returns the previous count
         */
        uint64_t set_count( uint64_t ucount );

        /*!
          clear_count - set count zero and returns the previous count
         */
        uint64_t clear_count( );

    private:
        std::condition_variable m_cv    ;
        std::mutex              m_lock  ;
        uint64_t                m_count ;

        // deleted constructors/assignmenet operators
        CLock() = delete;
        CLock( const CLock& other ) = delete;
        CLock& operator=( const CLock& opther ) = delete;
    };
}
