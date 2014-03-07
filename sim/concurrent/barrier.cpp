#include <chrono>
#include <mutex>
#include <thread>

#include "barrier.h"

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

    if (arrived == total)
    {
        arrived = 0;
        // TODO Delete. Creates time between steps.
        std::this_thread::sleep_for(std::chrono::seconds(1));
        condition.notify_all();
    }
    else
    {
        // Block all spurious wakeups.
        // TODO fix wait condition.
        condition.wait(lock, [=]{return false;});
    }
}
