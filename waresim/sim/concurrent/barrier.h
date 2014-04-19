#ifndef SIM_CONCURRENT_BARRIER_H
#define SIM_CONCURRENT_BARRIER_H

#include <condition_variable>
#include <mutex>

/**
 * A cyclic barrier with the option to wait before starting the next iteration.
 */
class Barrier
{
    public:
        Barrier(int, int);
        ~Barrier();

        void arrive();
    private:
        // The wait time (in milliseconds) before starting the next iteration.
        int wait_time;
        // Used when entering the barrier.
        int total;
        // How many threads have arrived at the barrier.
        int arrived = 0;
        // Used to prevent thread from leaving the barrier.
        int release = 0;
        // Lock on this mutex.
        std::mutex mutex;
        // Wait on this condition variable.
        std::condition_variable condition;
};

#endif
