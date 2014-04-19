#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <ctime>
#include <memory>
#include <string>

#include "algo/dispatch/dispatch_algo.h"
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
        // Set the log file directory.
        void set_log_dir(std::string dir);

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
        // The view to render the warehouse.
        View *view = nullptr;
        // The log file directory.
        std::string log_dir;
};

#endif
