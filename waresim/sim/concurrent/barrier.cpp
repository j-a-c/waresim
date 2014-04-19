#include <chrono>
#include <mutex>
#include <thread>

#include "barrier.h"

/**
 * Implementation of a cyclic non-spinning barrier.
 */

/**
 * Constructor.
 *
 * @param total The total number of threads using this barrier.
 * @param time The wait time before starting the next iteration.
 */
Barrier::Barrier(int total, int time)
{
    this->total = total;
    this->wait_time = time;
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
        // TODO Delete. Creates time between steps. This is not necessary for
        // the correctness of the algorithm. It is simply so the human eye can
        // perceive changes on the view.
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));

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
