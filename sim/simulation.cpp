#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>

#include "algo/rand_dispatch_algo.h"
#include "concurrent/barrier.h"
#include "dispatcher.h"
#include "factory.h"
#include "order_generator.h"
#include "rand/rand.h"
#include "simulation.h"

// For testing.
#include <iostream>
#include <unistd.h>

/**
 * Constructor.
 *
 * @param sim_length The length of the simulation in seconds.
 * @param factory_file The file containing the factory representation.
 */
Simulation::Simulation(int sim_length, std::string factory_file, 
        DispatchAlgo * dispatch_algo)
{
    this->sim_length = sim_length;

    this->factory = Factory::parse_default_factory(factory_file);

    this->dispatch_algo = dispatch_algo;
}

/**
 * Destructor.
 */
Simulation::~Simulation()
{
    delete barrier;
    delete order_gen;
    delete dispatcher;
}

/**
 * Returns the number of threads that need to be synchronized in this
 * simulation.
 */
int Simulation::num_threads()
{
    // TODO
    // Order generator, dispatcher.
    return 1 + 1;
}

/**
 * Starts the simulation in a separate thread.
 */
void Simulation::run()
{
    barrier = new Barrier(num_threads());

    // Set the start time for this simulation.
    start_time = time(nullptr);

    // Initialize the order generator.
    order_gen = new OrderGenerator(start_time, sim_length);
    order_gen->set_barrier(barrier);
    order_gen->set_rand(Rand(std::rand()));
    // Initialize workers.

    // Initialize dispatcher.
    dispatcher = new Dispatcher(start_time, sim_length);
    dispatcher->set_barrier(barrier);
    dispatcher->set_factory(&factory);
    dispatcher->set_order_generator(order_gen);
    dispatcher->set_algo(dispatch_algo);

    // Start the simulation.
    order_gen->run();
    dispatcher->run();
}

/**
 * Waits for the simulation to finish its execution.
 * Should only be called after run().
 */
void Simulation::join()
{
    order_gen->join();
    dispatcher->join();
}

