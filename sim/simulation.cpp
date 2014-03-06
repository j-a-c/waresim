#include <ctime>
#include <thread>

#include "dispatcher.h"
#include "order_generator.h"
#include "simulation.h"

// For testing.
#include <iostream>
#include <unistd.h>

/**
 * Constructor.
 *
 * @param sim_length The length of the simulation in seconds.
 */
Simulation::Simulation(int sim_length)
{
    this->sim_length = sim_length;
}

/**
 * Destructor.
 */
Simulation::~Simulation()
{
    delete order_gen;
    delete dispatcher;
}

/**
 * Starts the simulation in a separate thread.
 */
void Simulation::run()
{

    // Set the start time for this simulation.
    start_time = time(nullptr);

    // Initialize the order generator.
    order_gen = new OrderGenerator(start_time, sim_length);
    // Initialize workers.

    // Initialize dispatcher.
    dispatcher = new Dispatcher(start_time, sim_length);
    //dispatcher.set_factory(factory);
    dispatcher->set_order_generator(order_gen);
    //dispatcher.set_workers(workers);


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

