#include <cstdlib>
#include <iostream>
#include <string>

#include "parser/config_parser.h"
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
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: ./waresim config" << std::endl;
        return 1;
    }

    // Set up the configuration file parser and parse the config file.
    ConfigParser parser(std::string{argv[1]});
    parser.parse();

    // Get config parameters.
    int sim_length = parser.get_sim_length();
    std::string warehouse = parser.get_warehouse_file();
    unsigned int seed = parser.get_seed();
    // wait time in between steps (in msecs)
    
    // TODO Make a parameter.
    View *view = new OpenGLView();

    // Set random seed. All other random generators should be seeded in a
    // deterministic order using std::rand(). We reset the seed because the
    // view might set the seed for itself.
    std::srand(seed);

    // TODO Make a parameter.
    DispatchAlgo *dispatch_algo = new RandDispatchAlgo(Rand(std::rand()));
    // TODO Make a parameter.
    RoutingAlgo *routing_algo = new FCFSRoutingAlgo();


    Simulation sim(sim_length, warehouse);
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
