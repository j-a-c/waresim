#include <cstdlib>

#include "rand/rand.h"
#include "simulation.h"

// TODO Delete later, used for testing.
#include <iostream>
#include <unistd.h>

/**
 * Implementation for the simulation.
 */

/**
 * Constructor.
 *
 * @param sim_length The length of the simulation in seconds.
 * @param warehouse_file The file containing the warehouse representation.
 */
Simulation::Simulation(int sim_length, std::string warehouse_file) 
{
    this->sim_length = sim_length;

    this->warehouse = Warehouse::parse_default_warehouse(warehouse_file);
}

/**
 * Sets the dispatch algorithm to be used in this simulation.
 *
 * @param dispatch_algo The dispatch algorithm to be used in this simulation.
 */
void Simulation::set_dispatch_algo(DispatchAlgo* dispatch_algo)
{
    this->dispatch_algo = dispatch_algo;
}

/**
 * Sets the routing algorithm to be used in the simulation.
 *
 * @routing_algo The routing algorithm to be used in this simulation.
 */
void Simulation::set_routing_algo(RoutingAlgo *routing_algo)
{
    this->routing_algo = routing_algo;
}

void Simulation::set_view(View * view)
{
    this->view = view;
}

/**
 * Destructor.
 */
Simulation::~Simulation()
{
    // Delete memory this object allocated.
    delete barrier;
    delete order_gen;
    delete scheduler;
}

/**
 * Returns the number of threads that need to be synchronized in this
 * simulation.
 */
int Simulation::num_threads()
{
    // TODO Make this neater.
    // Order generator, dispatcher, scheduler.
    return 1 + 1 + 1;
}

/**
 * Starts the simulation in a separate thread.
 */
void Simulation::start()
{

    // This is the barrier the different components in the simulation will
    // synchronize upon. We use a barrier to simulate 'time steps' within the
    // warehouse.
    barrier = new Barrier(num_threads());

    // Set the start time for this simulation.
    start_time = time(nullptr);

    // Initialize the order generator.
    order_gen = new OrderGenerator(start_time, sim_length);
    order_gen->set_barrier(barrier);
    order_gen->set_rand(Rand(std::rand()));
    order_gen->set_warehouse(&warehouse);
    order_gen->set_log_dir(log_dir + "order_gen.txt");

    // Initialize dispatcher.
    dispatcher = new Dispatcher(start_time, sim_length);
    dispatcher->set_barrier(barrier);
    dispatcher->set_warehouse(&warehouse);
    dispatcher->set_order_generator(order_gen);
    dispatcher->set_algo(dispatch_algo);
    dispatcher->set_log_dir(log_dir + "dispatcher.txt");

    // Initialize the scheduler.
    scheduler = new Scheduler(start_time, sim_length);
    scheduler->set_barrier(barrier);
    scheduler->set_warehouse(&warehouse);
    scheduler->set_rand(Rand(std::rand()));
    scheduler->set_algo(routing_algo);
    scheduler->set_log_dir(log_dir + "scheduler.txt");

    // Initial the render view.
    view->set_sim_params(start_time, sim_length);
    view->set_warehouse(&warehouse);
    view->setup();

    // Start the simulation.
    order_gen->start();
    dispatcher->start();
    scheduler->start();
    view->run();
}

/**
 * Set the log file directory.
 *
 * @param dir The log file directory to use.
 */
void Simulation::set_log_dir(std::string dir)
{
    this->log_dir = dir;
    log_dir.append("/");
}

/**
 * Waits for the simulation to finish its execution.
 * Should only be called after run().
 */
void Simulation::join()
{
    order_gen->join();
    dispatcher->join();
    scheduler->join();
    //view->join();
}

