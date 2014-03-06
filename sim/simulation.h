#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <ctime>
#include <memory>
#include <thread>

#include "dispatcher.h"
#include "order_generator.h"

/**
 * @author Joshua A. Campbell
 *
 * Holds all the data concerning the simulation.
 */
class Simulation
{
    public:
        Simulation(int);
        ~Simulation();

        // Runs the simulation.
        void run();
        // Waits for the simulation thread to finish.
        void join();
    private:
        // The start time of the simulation.
        time_t start_time;
        // The length of the simulation in seconds.
        int sim_length;
        // Thread for running the simulation.
        std::thread simulation;
        // Generates orders for this simulation.
        OrderGenerator *order_gen;
        // Dispatcher for the simulation.
        Dispatcher *dispatcher;
};

#endif
