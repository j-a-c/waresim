#ifndef SIM_CONCURRENT_BARRIER_H
#define SIM_CONCURRENT_BARRIER_H

#include <condition_variable>
#include <mutex>

/**
 * A cyclic barrier.
 */
class Barrier
{
    public:
        Barrier(int);
        ~Barrier();

        void arrive();
    private:
        // Used when entering the barrier.
        int total;
        int arrived = 0;
        // Used to prevent thread from leaving the barrier.
        int release = 0;
        // Lock on this mutex.
        std::mutex mutex;
        // Wait on this condition variable.
        std::condition_variable condition;
};

#endif
