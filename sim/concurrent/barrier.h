#ifndef SIM_CONCURRENT_BARRIER_H
#define SIM_CONCURRENT_BARRIER_H

#include <condition_variable>
#include <mutex>

/**
 * A barrier class. 
 */
class Barrier
{
    public:
        Barrier(int);
        ~Barrier();

        void arrive();
    private:
        int total;
        int arrived = 0;

        std::mutex mutex;
        std::condition_variable condition;
};

#endif
