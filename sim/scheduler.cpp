#include <ctime>

#include "scheduler.h"

// TODO Delete later, used for debug.
#include <iostream>

/**
 * Constructor.
 *
 * @param start_time
 * @param sim_length
 */
Scheduler::Scheduler(time_t start_time, int sim_length)
{
    this->start_time = start_time;
    this->sim_length = sim_length;
}

/**
 * Private method that encapsulates the scheduling algorithm so it can be run in
 * a thread.
 */
void Scheduler::simulate()
{
    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(time(nullptr), start_time) < sim_length)
    {
        // TODO Implement.
        std::cout << "Scheduling" << std::endl;
        
        barrier->arrive();
    }
}

/**
 * Assign the factory used in the simulation.
 *
 * @param factory The factory to use.
 */
void Scheduler::set_factory(Factory *factory)
{
    this->factory = factory;
}


/**
 * Set the barrier to synchronize on.
 */
void Scheduler::set_barrier(Barrier *barrier)
{
    this->barrier = barrier;
}
