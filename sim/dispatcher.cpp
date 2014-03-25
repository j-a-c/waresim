#include <ctime>

#include "dispatcher.h"

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
void Dispatcher::run()
{
    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(time(nullptr), start_time) < sim_length)
    {
        // TODO Implement simulation stuff.
        if (order_gen->has_order())
        {
            std::cout << "Dispatch found new order." << std::endl;
            algo->assign_order(this->warehouse, order_gen->get_order());
        }
        
        barrier->arrive();
    }
}

/**
 * Sets the warehouse used by the dispatcher.
 */
void Dispatcher::set_warehouse(Warehouse * f)
{
    this->warehouse = f;
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

void Dispatcher::set_algo(DispatchAlgo* a)
{
    this->algo = a;
}
