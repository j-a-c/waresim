#ifndef SIM_ORDER_GENERATOR_H
#define SIM_ORDER_GENERATOR_H

#include <condition_variable>
#include <ctime>
#include <mutex>
#include <queue>

#include "concurrent/barrier.h"
#include "warehouse.h"
#include "order.h"
#include "rand/rand.h"
#include "thread/thread.h"

/**
 * @author Joshua A. Campbell
 *
 * Generates orders within the simulation.
 */
class OrderGenerator : public Thread
{
    public:
        OrderGenerator(time_t, int);
 
        // Set the barrier.
        void set_barrier(Barrier *);
        // Set the random number generator.
        void set_rand(Rand);
        // Set the warehouse.
        void set_warehouse(Warehouse *);
        // Returns true if there is an order available.
        bool has_order();
        // Returns the next order or blocks if there are no orders.
        Order get_order();
    private:
        // Override from Thread.
        void run() override;

        // Adds an order to the order queue.
        void add_order(const Order);
        
        // The start time of the simulation.
        time_t start_time;
        // The length of the simulation in seconds.
        int sim_length;
        // Holds the order that were generated.
        std::queue<Order> orders;
        // Mutex for the order queue. Used as a locking point.
        std::mutex orders_mutex;
        // Condition variable. Used for blocking if queue is empty.
        std::condition_variable order_condition;
        // The barrier.
        Barrier *barrier = nullptr;
        // The random number generator.
        Rand rand;
        // The warehouse to choose bins from.
        Warehouse *warehouse;
};

#endif
