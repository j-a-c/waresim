#include <cstdlib>
#include <iostream>
#include <string>

#include "parser/config_parser.h"
#include "sim/algo/contention/backoff_algo.h"
#include "sim/algo/contention/contention_algo.h"
#include "sim/algo/dispatch/dispatch_algo.h"
#include "sim/algo/dispatch/rand_dispatch_algo.h"
#include "sim/algo/ordering/order_algo.h"
#include "sim/algo/ordering/rand_order_algo.h"
#include "sim/algo/path/path_algo.h"
#include "sim/algo/path/shortest_path_algo.h"
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
    // These should be self-explanatory, but for more details see
    // config_parser.h
    int sim_length              = parser.get_sim_length();
    std::string warehouse       = parser.get_warehouse_file();
    unsigned int seed           = parser.get_seed();
    std::string log_dir         = parser.get_log_dir();
    std::string view_param      = parser.get_view();
    int wait_time               = parser.get_wait_time();
    std::string dispatch_param  = parser.get_dispatch_algo();
    std::string routing_param   = parser.get_routing_algo();
    std::string path_param      = parser.get_path_algo();
    std::string order_param     = parser.get_order_algo();
    std::string contention      = parser.get_contention_algo();
    double decay_factor         = parser.get_decay_factor();
    
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

    // Add statements as more dispatch algorithms are added.
    DispatchAlgo *dispatch_algo = new RandDispatchAlgo(Rand(std::rand()));

    // Add statements as more routing algorithms are added.
    RoutingAlgo *routing_algo = new FCFSRoutingAlgo();

    // Add statements as more pathfinding algorithms are added.
    PathAlgo *path_algo = new ShortestPathAlgo(Rand(std::rand()));

    // Add statements as more ordering algorithms are added.
    OrderAlgo *order_algo = new RandOrderAlgo(Rand(std::rand()));

    // Add statements as more contention algorithms are added.
    ContentionAlgo *contention_algo = new BackoffAlgo();

    // Set up the simulation.
    Simulation sim(sim_length, warehouse);
    sim.set_contention_algo(contention_algo);
    sim.set_dispatch_algo(dispatch_algo);
    sim.set_routing_algo(routing_algo);
    sim.set_order_algo(order_algo);
    sim.set_path_algo(path_algo);
    sim.set_view(view);
    sim.set_log_dir(log_dir);
    sim.set_wait_time(wait_time);
    sim.set_decay_factor(decay_factor);

    // Start the simulation and wait for it to finish.
    sim.start();
    sim.join();

    // Delete allocated memory.
    delete dispatch_algo;
    delete routing_algo;
    delete path_algo;
    delete order_algo;
    delete contention_algo;
    delete view;

    return 0;
}
