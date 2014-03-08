#include <chrono>
#include <mutex>
#include <thread>

#include "barrier.h"

// TODO Delete later, used for debug.
#include <iostream>

/**
 * Implementation of a cyclic non-spinning barrier.
 */

/**
 * Constructor.
 */
Barrier::Barrier(int total)
{
    this->total = total;
}

/**
 * Destructor.
 */
Barrier::~Barrier()
{

}

void Barrier::arrive()
{
    std::unique_lock<std::mutex> lock(mutex);

    arrived++;
    int old_r = release;

    if (arrived == total)
    {
        std::cout << "=== New Step ===" << std::endl;
        // TODO Delete. Creates time between steps.
        std::this_thread::sleep_for(std::chrono::seconds(1));

        arrived = 0;
        release = (release + 1) % 2;
        condition.notify_all();
    }
    else
    {
        // Block all spurious wakeups.
        // TODO fix wait condition.
        condition.wait(lock, [=]{return old_r != release;});
    }
}
