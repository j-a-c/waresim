#include <cstdlib>
#include <string>

#include "sim/algo/dispatch/dispatch_algo.h"
#include "sim/algo/dispatch/rand_dispatch_algo.h"
#include "sim/algo/routing/routing_algo.h"
#include "sim/algo/routing/fcfs_routing_algo.h"
#include "sim/simulation.h"
#include "sim/view/opengl_view.h"
#include "sim/view/view.h"

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
    // TODO Make a parameter.
    View *view = new OpenGLView();


    Simulation sim(sim_length, factory_file);
    sim.set_dispatch_algo(dispatch_algo);
    sim.set_routing_algo(routing_algo);
    sim.set_view(view);

    sim.start();
    sim.join();

    delete dispatch_algo;
    delete routing_algo;
    delete view;

    return 0;
}
