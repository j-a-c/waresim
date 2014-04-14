#include "order_generator.h"

// TODO For debug purposes. Delete later.
#include <iostream>

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
    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(time(nullptr), start_time) < sim_length)
    { 

        // TODO Add a random scheme.
        // Generate a new order every time step.
        double d = rand.rand();
        std::cout << "Order rand: " << d << std::endl;

        // Each time step we have a 50% chance of generating a new order.
        if (d < 0.50)
        {
            // Get the location of all the bins in the warehouse.
            auto bins = warehouse->get_bins();

            // Select a random bin index.
            int index = rand.rand() * bins.size();

            std::cout << "Creating new order: " << bins[index] << std::endl;

            // Add a new order with the selected bin's location.
            add_order(Order(bins[index]));
        }

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
 * Set the random number generator.
 *
 * @param rand The random number generator.
 */
void OrderGenerator::set_rand(Rand rand)
{
    this->rand = rand;
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
