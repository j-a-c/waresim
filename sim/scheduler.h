#ifndef SIM_SCHEDULER_H
#define SIM_SCHEDULER_H

#include <ctime>
#include <vector>

#include "algo/routing/routing_algo.h"
#include "concurrent/barrier.h"
#include "factory.h"
#include "rand/rand.h"
#include "thread/thread.h"

/**
 * Reponsible for scheduling the worker's movements in the factory.
 */
class Scheduler : public Thread
{
    public:
        Scheduler(time_t, int);

        // Set the factory.
        void set_factory(Factory *);
        // Set the barrier.
        void set_barrier(Barrier *);
        // Set the random number generator.
        void set_rand(Rand);
        // Set the routing algorithm.
        void set_algo(RoutingAlgo *);

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
        // The factory.
        Factory *factory = nullptr;
        // The barrier.
        Barrier *barrier = nullptr;
        // The random number generator.
        Rand rand;
        // Routing algorithm.
        RoutingAlgo *routing_algo = nullptr;

};

#endif
