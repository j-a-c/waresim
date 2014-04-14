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

/**
 * Called when a thread arrives at the barrier. The thread will wait, unless it
 * is the last thread to arrive at the barrier. If the thread is the last
 * thread, it will notify all the other threads.
 */
void Barrier::arrive()
{
    // Acquire the lock before modifying anything.
    std::unique_lock<std::mutex> lock(mutex);

    arrived++;
    int old_r = release;

    if (arrived == total)
    {
        std::cout << "=== Ending Step ===" << std::endl;
        // TODO Delete. Creates time between steps. This is not necessary for
        // the correctness of the algorithm. It is simply so the human eye can
        // perceive changes on the view.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Reset the number of arrived threads.
        arrived = 0;

        // Swith the release variable so threads can continue after we notify
        // them.
        release = (release + 1) % 2;
        // Notify all waiting threads.
        condition.notify_all();
    }
    else
    {
        // Block all spurious wakeups. The thread will wait until the last
        // thread changes the release variable.
        condition.wait(lock, [=]{return old_r != release;});
    }
}
