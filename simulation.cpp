#include <ctime>
#include <thread>

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
 * Private method that encapsulates the simulation so it can be run in a
 * thread.
 */
void Simulation::simulate()
{
    

    std::cout << "Starting simulation." << std::endl;

    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(time(nullptr), start_time) < sim_length)
    {
        // TODO Implement simulation stuff.
        if (order_gen->has_order())
        {
            std::cout << "has order" << std::endl;
            order_gen->get_order();
            sleep(1);
        }
    }

    std::cout << "Ending simulation." << std::endl;
}

/**
 * Starts the simulation in a separate thread.
 */
void Simulation::run()
{

    // Set the start time for this simulation.
    start_time = time(nullptr);

    // Initialize the order generator.
    order_gen.reset(new OrderGenerator(start_time, sim_length));

    // Start the simulation.
    simulation = std::thread(&Simulation::simulate, this);
    order_gen->run();
}

/**
 * Waits for the simulation to finish its execution.
 * Should only be called after run().
 */
void Simulation::join()
{
    order_gen->join();

    simulation.join();
}

