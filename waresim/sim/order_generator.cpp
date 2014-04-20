#include "order_generator.h"

#include <string>

/**
 * Constructor. Sets the start time and length for the simulation.
 *
 * @param start_time The simulation's start time.
 * @param length The length of the simulation in seconds.
 */
OrderGenerator::OrderGenerator(time_t start_time, int sim_length)
{
    this->start_time = start_time;
    this->sim_length = sim_length;
}

/**
 * Encapsulates the order generation so it can be run in a separate thread.
 * The default order generator has a 50% chance of generating a new order each
 * time step.
 */
void OrderGenerator::run()
{
    // Will be used for logging message.
    std::string log_msg;

    std::string log_iter_end = std::string("=====");

    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(time(nullptr), start_time) < sim_length)
    { 

        if (order_algo->order_ready())
        {
            // Get the new order.
            Order order = order_algo->get_new_order(warehouse);

            // Log that a new order is ready.
            log_msg = std::string("A new order is ready at :");
            log_msg.append(std::to_string(order.get_pos()));
            logger.log(log_msg);

            // Add the order to the queue.
            add_order(order);
        }

        logger.log(log_iter_end);

        barrier->arrive();
    }

}

/**
 * Returns true if there is an order available.
 *
 * @return true if there is an order available.
 */
bool OrderGenerator::has_order()
{
    orders_mutex.lock();
    
    // Get the return value.
    bool ret = !orders.empty();

    orders_mutex.unlock();

    return ret;
}

/**
 * Returns the next order or blocks if there is none available.
 */
Order OrderGenerator::get_order()
{
    // Lock on the orders mutex.
    std::unique_lock<std::mutex> lock(orders_mutex);
    // Wait until there is one order. Release the previous lock.
    order_condition.wait(lock, [=]{ return !orders.empty(); });
    // Return front order.
    Order ret(std::move(orders.front()));
    orders.pop();
    return ret;
}

/**
 * Adds an order to the order queue.
 *
 * @param order The order to add to the queue.
 */
void OrderGenerator::add_order(const Order order)
{
    // Lock the orders mutex.
    std::unique_lock<std::mutex> lock(orders_mutex);
    
    // Add an order and notify anyone who is waiting.
    orders.push(order);
    order_condition.notify_one();

}

/**
 * Set the barrier to synchronize on.
 *
 * @param b The barrier to synchronize on.
 */
void OrderGenerator::set_barrier(Barrier *b)
{
    this->barrier = b;
}

/**
 * Set the order generating algorithm to use.
 *
 * @param algo The order generating algorithm to use.
 */
void OrderGenerator::set_order_algo(OrderAlgo *algo)
{
    this->order_algo = algo;
}

/**
 * Set the warehouse used by simulation.
 *
 * @param warehouse The warehouse used by simulation.
 */
void OrderGenerator::set_warehouse(Warehouse *warehouse)
{
    this->warehouse = warehouse;
}

/**
 * Set the log file directory.
 *
 * @param dir The new log file directory.
 */
void OrderGenerator::set_log_dir(std::string dir)
{
    logger.set_up(dir);
}
