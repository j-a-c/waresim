#include <cstdlib>
#include <iostream>
#include <string>

#include "parser/config_parser.h"
#include "sim/algo/dispatch/dispatch_algo.h"
#include "sim/algo/dispatch/rand_dispatch_algo.h"
#include "sim/algo/routing/routing_algo.h"
#include "sim/algo/routing/fcfs_routing_algo.h"
#include "sim/simulation.h"
#include "sim/view/empty_view.h"
#include "sim/view/opengl_view.h"
#include "sim/view/view.h"

/**
 * @author Joshua A. Campbell
 *
 * Driver for Waresim's simulation.
 * This is where execution will start.
 * There should be one argument that contains the location of the config file
 * to parse.
 */
int main(int argc, char **argv)
{
    // Simple argument check.
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
    std::string log_dir = parser.get_log_dir();
    std::string view_param = parser.get_view();
    
    // Set up the view.
    View *view;
    if (view_param == "OpenGL")
        view = new OpenGLView();
    else
        view = new EmptyView();

    // Set random seed. All other random generators should be seeded in a
    // deterministic order using std::rand(). We reset the seed because the
    // view might set the seed for itself.
    std::srand(seed);

    /*
     * If you are setting custom dispatch and routing algorithms, edit below.
     */

    // TODO Make a parameter.
    DispatchAlgo *dispatch_algo = new RandDispatchAlgo(Rand(std::rand()));
    // TODO Make a parameter.
    RoutingAlgo *routing_algo = new FCFSRoutingAlgo();

    // Set up the simulation.
    Simulation sim(sim_length, warehouse);
    sim.set_dispatch_algo(dispatch_algo);
    sim.set_routing_algo(routing_algo);
    sim.set_view(view);
    sim.set_log_dir(log_dir);

    // Start the simulation and wait for it to finish.
    sim.start();
    sim.join();

    // Delete allocated memory.
    delete dispatch_algo;
    delete routing_algo;
    delete view;

    return 0;
}
