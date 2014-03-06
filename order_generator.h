#ifndef ORDER_GENERATOR_H
#define ORDER_GENERATOR_H

#include <condition_variable>
#include <ctime>
#include <mutex>
#include <thread>
#include <queue>
#include "order.h"

class OrderGenerator
{
    public:
        OrderGenerator(time_t, int);
        // Runs the order generation in a separate thread.
        void run();
        // Waits for the order generator to stop generating orders.
        void join(); 
        // Returns true if there is an order available.
        bool has_order();
        // Returns the next order or blocks if there are no orders.
        Order get_order();
    private:
        // Encapsulates the order generation so it can be run in a separate 
        // thread.
        void generate();
        // Adds an order to the order queue.
        void add_order(const Order);
        // Thread to generate orders.
        
        // The start time of the simulation.
        time_t start_time;
        // The length of the simulation in seconds.
        int sim_length;
        std::thread generator;
        // Holds the order that were generated.
        std::queue<Order> orders;
        // Mutex for the order queue. Used as a locking point.
        std::mutex orders_mutex;
        // Condition variable. Used for blocking if queue is empty.
        std::condition_variable order_condition;
};

#endif
