#ifndef SIM_DISPATCHER_H
#define SIM_DISPATCHER_H

#include <ctime>

#include "algo/dispatch/dispatch_algo.h"
#include "concurrent/barrier.h"
#include "warehouse.h"
#include "order_generator.h"
#include "thread/thread.h"

/**
 * @author Joshua A. Campbell
 *
 * Calculates the dispatching algorithm for the simulation.
 */
class Dispatcher : public Thread
{
    public:
        Dispatcher(time_t, int);

        // Set the warehouse.
        void set_warehouse(Warehouse *);
        // Set the order generator.
        void set_order_generator(OrderGenerator *);
        // Set the barrier.
        void set_barrier(Barrier *);
        // Set the dispatch algorithm.
        void set_algo(DispatchAlgo *);
    private:
        // Override from Thread.
        void run() override;

        // The start time of the simulation.
        time_t start_time;
        // The length of the simulation in seconds.
        int sim_length;
        // The warehouse;
        Warehouse *warehouse = nullptr;
        // The order generator.
        OrderGenerator *order_gen = nullptr;
        // The barrier to synchronize on.
        Barrier *barrier = nullptr;
        // Dispatching algorithm.
        DispatchAlgo *algo = nullptr;
};

#endif
