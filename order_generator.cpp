#include <condition_variable>
#include <ctime>
#include <thread>
#include <queue>
#include "order.h"
#include "order_generator.h"

// For testing.
#include <iostream>

/**
 * Constructor.
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
 */
void OrderGenerator::generate()
{
    std::cout << "Starting order generation." << std::endl;

    // The last time we generated an order.
    time_t last_gen_time = 0;

    time_t now;

    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(now=time(nullptr), start_time) < sim_length)
    { 

        // TODO Implement a random scheme.
        // Generate an order at least every second.
        if (difftime(now,last_gen_time) > 1)
        {
            add_order(Order());

            // Get current time since there are multiple thread.
            last_gen_time = time(nullptr);
        }
    }

    std::cout << "Ending order generation." << std::endl;
}

/**
 * Returns true if there is an order available.
 */
bool OrderGenerator::has_order()
{
    orders_mutex.lock();
    
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
 * Runs the order generation in a separate thread.
 */
void OrderGenerator::run()
{
    generator = std::thread(&OrderGenerator::generate, this);  
}

/**
 * Waits for the order generator to stop generating orders.
 */
void OrderGenerator::join()
{
    generator.join();
}
