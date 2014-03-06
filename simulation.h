#ifndef SIMULATION_H
#define SIMULATION_H

#include <ctime>
#include <memory>
#include <thread>
#include "order_generator.h"

class Simulation
{
    public:
        Simulation(int);
        // Runs the simulation in a separate thread.
        void run();
        // Waits for the simulation thread to finish.
        void join();
    private:
        // Encapsulates the simulation so it can be run from a thread.
        void simulate();

        // The start time of the simulation.
        time_t start_time;
        // The length of the simulation in seconds.
        int sim_length;
        // Thread for running the simulation.
        std::thread simulation;
        // Generates orders for this simulation.
        std::unique_ptr<OrderGenerator> order_gen;
};

#endif
