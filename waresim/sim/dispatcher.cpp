#include <ctime>

#include "dispatcher.h"

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
    // Will be used for logging message.
    std::string log_msg;
    std::string log_iter_end = std::string("=====");

    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(time(nullptr), start_time) < sim_length)
    {
        if (order_gen->has_order())
        {
            // Log that we have a new order.
            log_msg = std::string{"Dispach has new order. Assigned to worker #: "};

            int id = algo->assign_order(this->warehouse, order_gen->get_order());

            log_msg.append(std::to_string(id));
            logger.log(log_msg);
        }
        
        logger.log(log_iter_end);

        barrier->arrive();
    }
}

/**
 * Sets the warehouse used by the dispatcher.
 *
 * @param w The warehouse to be used by the dispatcher.
 */
void Dispatcher::set_warehouse(Warehouse * w)
{
    this->warehouse = w;
}

/**
 * Sets the order generator used by the dispatcher.
 *
 * @param o The order generator to be used by the dispatcher.
 */
void Dispatcher::set_order_generator(OrderGenerator *o)
{
    this->order_gen = o;
}

/**
 * Sets the barrier to synchronize upon.
 *
 * @param b The barrier to synchronize on.
 */
void Dispatcher::set_barrier(Barrier * b)
{
    this->barrier = b;
}

/**
 * Sets the dispatch algorithm to be used by the dispatcher.
 *
 * @param a The dispatch algorithm to be used by the dispatcher.
 */
void Dispatcher::set_algo(DispatchAlgo* a)
{
    this->algo = a;
}

/**
 * Set the log file directory.
 *
 * @param dir The new log file directory.
 */
void Dispatcher::set_log_dir(std::string dir)
{
    logger.set_up(dir);
}
