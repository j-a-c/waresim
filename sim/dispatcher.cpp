#include <ctime>

#include "concurrent/barrier.h"
#include "dispatcher.h"
#include "factory.h"
#include "order_generator.h"
#include "simulator.h"

// TODO For debug purposes. (Delete later)
#include <iostream>

/**
 * Constructor.
 *
 * @param start_time The start time of the simulation.
 * @param sim_length The length of the simulation in seconds.
 */
Dispatcher::Dispatcher(time_t start_time, int sim_length)
{
    this->start_time = start_time;
    this->sim_length = sim_length;
}

/**
 * Private method that encapsulates the dispatch algorithm so it can be run in
 * a thread.
 */
void Dispatcher::simulate()
{
    std::cout << "Starting dispatch." << std::endl;

    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(time(nullptr), start_time) < sim_length)
    {
        // TODO Implement simulation stuff.
        if (order_gen->has_order())
        {
            std::cout << "has order" << std::endl;
            order_gen->get_order();
        }
        
        std::cout << "Dispatcher is arriving at barrier." << std::endl;
        barrier->arrive();
    }

    std::cout << "Ending dispatch." << std::endl;
}

/**
 * Sets the factory used by the dispatcher.
 */
void Dispatcher::set_factory(Factory * f)
{
    this->factory = f;
}

/**
 * Sets the order generator used by the dispatcher.
 */
void Dispatcher::set_order_generator(OrderGenerator *o)
{
    this->order_gen = o;
}

/**
 * Sets the barrier to synchronize upon.
 */
void Dispatcher::set_barrier(Barrier * b)
{
    this->barrier = b;
}
