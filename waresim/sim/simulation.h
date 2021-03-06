#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <ctime>
#include <memory>
#include <string>

#include "algo/contention/contention_algo.h"
#include "algo/dispatch/dispatch_algo.h"
#include "algo/ordering/order_algo.h"
#include "algo/path/path_algo.h"
#include "algo/routing/routing_algo.h"
#include "concurrent/barrier.h"
#include "dispatcher.h"
#include "warehouse.h"
#include "order_generator.h"
#include "scheduler.h"
#include "view/view.h"

/**
 * @author Joshua A. Campbell
 *
 * Holds all the data concerning the simulation.
 */
class Simulation
{
    public:
        Simulation(int, std::string);
        ~Simulation();

        // Starts the simulation.
        void start();
        // Waits for the simulation thread to finish.
        void join();

        // Set the dispatch algorithm to use.
        void set_dispatch_algo(DispatchAlgo *);
        // Set the routing algorithm to use.
        void set_routing_algo(RoutingAlgo *);
        // Set the view to render the simulation.
        void set_view(View *);
        // Set the pathfinding algorithm to use.
        void set_path_algo(PathAlgo *);
        // Set the order generating algorithm to use.
        void set_order_algo(OrderAlgo *);
        // Set the contention algorithm to use.
        void set_contention_algo(ContentionAlgo *);
        // Set the log file directory.
        void set_log_dir(std::string dir);
        // Set the wait time between steps.
        void set_wait_time(int);
        // Set the decay factor.
        void set_decay_factor(double);

    private:
        // Returns the number of threads that need to by synchronized.
        int num_threads();

        // The start time of the simulation.
        time_t start_time;
        // The length of the simulation in seconds.
        int sim_length;
        // Seed for the simulation.
        unsigned int seed;
        // Generates orders for this simulation.
        OrderGenerator *order_gen = nullptr;
        // Dispatcher for the simulation.
        Dispatcher *dispatcher = nullptr;
        // Scheduler for the simulation.
        Scheduler *scheduler = nullptr;
        // Warehouse that the dispatcher will use.
        Warehouse warehouse;
        // The barrier.
        Barrier *barrier = nullptr;
        // The dispatch algorithm.
        DispatchAlgo *dispatch_algo = nullptr;
        // The routing algorithm.
        RoutingAlgo *routing_algo = nullptr;
        // The pathfinding algorithm.
        PathAlgo *path_algo = nullptr;
        // The contention algorithm to use.
        ContentionAlgo *contention_algo = nullptr;
        // The order generating algorithm to use.
        OrderAlgo *order_algo = nullptr;
        // The view to render the warehouse.
        View *view = nullptr;
        // The log file directory.
        std::string log_dir;
        // The wait time between steps.
        int wait_time = 1000;
        // The decay factor for windows.
        double decay_factor = 0.9;
};

#endif
