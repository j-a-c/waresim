#include <cstdlib>

#include "rand/rand.h"
#include "simulation.h"

// TODO Delete later, used for testing.
#include <iostream>
#include <unistd.h>

/**
 * Implementation for Simulation.
 */

/**
 * Constructor.
 *
 * @param sim_length The length of the simulation in seconds.
 * @param factory_file The file containing the factory representation.
 */
Simulation::Simulation(int sim_length, std::string factory_file) 
{
    this->sim_length = sim_length;

    this->factory = Factory::parse_default_factory(factory_file);
}

/**
 * Sets the dispatch algorithm to be used in this simulation.
 */
void Simulation::set_dispatch_algo(DispatchAlgo* dispatch_algo)
{
    this->dispatch_algo = dispatch_algo;
}

/**
 * Sets the routing algorithm to be used in the simulation.
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
    delete barrier;
    delete order_gen;
    delete dispatcher;
    delete scheduler;
}

/**
 * Returns the number of threads that need to be synchronized in this
 * simulation.
 */
int Simulation::num_threads()
{
    // TODO
    // Order generator, dispatcher, scheduler.
    return 1 + 1 + 1;
}

/**
 * Starts the simulation in a separate thread.
 */
void Simulation::start()
{

    barrier = new Barrier(num_threads());

    // Set the start time for this simulation.
    start_time = time(nullptr);

    // Initialize the order generator.
    order_gen = new OrderGenerator(start_time, sim_length);
    order_gen->set_barrier(barrier);
    order_gen->set_rand(Rand(std::rand()));
    order_gen->set_factory(&factory);
    // Initialize workers.

    // Initialize dispatcher.
    dispatcher = new Dispatcher(start_time, sim_length);
    dispatcher->set_barrier(barrier);
    dispatcher->set_factory(&factory);
    dispatcher->set_order_generator(order_gen);
    dispatcher->set_algo(dispatch_algo);

    // Initialize the scheduler.
    scheduler = new Scheduler(start_time, sim_length);
    scheduler->set_barrier(barrier);
    scheduler->set_factory(&factory);
    scheduler->set_rand(Rand(std::rand()));
    scheduler->set_algo(routing_algo);


    // Initial the render view.
    view->set_sim_params(start_time, sim_length);
    view->set_factory(&factory);

    // Start the simulation.
    order_gen->start();
    dispatcher->start();
    scheduler->start();
    view->run();
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

