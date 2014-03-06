#include <condition_variable>
#include <chrono>
#include <ctime>
#include <queue>

#include "order.h"
#include "order_generator.h"
#include "simulation.h"

// TODO For debug purposes. Delete later.
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
void OrderGenerator::simulate()
{
    std::cout << "Starting order generation." << std::endl;

    time_t now;

    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(now=time(nullptr), start_time) < sim_length)
    { 

        // TODO Add a random scheme.
        // Generate a new order every time step.
        add_order(Order());

        std::this_thread::sleep_for(std::chrono::seconds(TIME_STEP));

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

