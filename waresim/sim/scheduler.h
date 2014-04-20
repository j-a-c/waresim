#ifndef SIM_SCHEDULER_H
#define SIM_SCHEDULER_H

#include <ctime>
#include <string>
#include <vector>

#include "algo/contention/contention_algo.h"
#include "algo/path/path_algo.h"
#include "algo/routing/routing_algo.h"
#include "concurrent/barrier.h"
#include "warehouse.h"
#include "rand/rand.h"
#include "thread/thread.h"
#include "../logger/logger.h"

/**
 * Reponsible for scheduling the worker's movements in the warehouse.
 */
class Scheduler : public Thread
{
    public:
        Scheduler(time_t, int);

        // Set the warehouse.
        void set_warehouse(Warehouse *);
        // Set the barrier.
        void set_barrier(Barrier *);
        // Set the random number generator.
        void set_rand(Rand);
        // Set the routing algorithm.
        void set_routing_algo(RoutingAlgo *);
        // Set the pathfinding algorithm.
        void set_path_algo(PathAlgo *);
        // Set the contention algorithm.
        void set_contention_algo(ContentionAlgo *);
        // Set the log file directory.
        void set_log_dir(std::string dir);
    private:
        // Override from Thread
        void run() override;
        // Returns a integer in the range [0, int).
        int rand_int(int);
        // Returns the shortest path between the two positions.
        std::vector<int> shortest_path(int, int);

        // The start time for the simulation.
        time_t start_time;
        // The length of the simulation.
        int sim_length;
        // The warehouse.
        Warehouse *warehouse = nullptr;
        // The barrier.
        Barrier *barrier = nullptr;
        // The random number generator.
        Rand rand;
        // Routing algorithm.
        RoutingAlgo *routing_algo = nullptr;
        // The pathfinding algorithm.
        PathAlgo *path_algo = nullptr;
        // The contention algorithm.
        ContentionAlgo *contention_algo = nullptr;
        // The logger.
        Logger logger;
};

#endif
