#include <cstdlib>
#include <string>

#include "sim/simulation.h"
#include "sim/algo/dispatch/dispatch_algo.h"
#include "sim/algo/dispatch/rand_dispatch_algo.h"
#include "sim/algo/routing/routing_algo.h"
#include "sim/algo/routing/fcfs_routing_algo.h"


/**
 * @author Joshua A. Campbell
 *
 * Driver for Waresim's simulation.
 */
int main()
{
    // TODO Create a config file?
    // Begin config
    int sim_length = 10;
    std::string factory_file = "factory.txt";
    unsigned int seed = 1000;
    // End config
    
    // Set random seed. All other random generators should be seeded in a
    // deterministic order using std::rand();
    std::srand(seed);

    // TODO Make a parameter.
    DispatchAlgo *dispatch_algo = new RandDispatchAlgo(Rand(std::rand()));
    // TODO Make a parameter.
    RoutingAlgo *routing_algo = new FCFSRoutingAlgo();

    Simulation sim(sim_length, factory_file, dispatch_algo, routing_algo);

    sim.run();
    sim.join();

    delete dispatch_algo;
    delete routing_algo;

    return 0;
}
