#include "thread.h"

/**
 * Implementation for Simulator.
 */

Thread::~Thread(){}

/**
 * Called to start the thread. Runs the run() methods within the new thread.
 */
void Thread::start()
{
    thread = std::thread(&Thread::run, this);
}

/**
 * Called to wait for the thread to terminate. Waits on the thread started in
 * start().
 */
void Thread::join()
{
    thread.join();
}

