#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <ctime>
#include <memory>
#include <string>
#include <thread>

#include "concurrent/barrier.h"
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
        Simulation(int, std::string);
        ~Simulation();

        // Runs the simulation.
        void run();
        // Waits for the simulation thread to finish.
        void join();
    private:
        // Returns the number of threads that need to by synchronized.
        int num_threads();

        // The start time of the simulation.
        time_t start_time;
        // The length of the simulation in seconds.
        int sim_length;
        // Seed for the simulation.
        unsigned int seed;
        // Thread for running the simulation.
        std::thread simulation;
        // Generates orders for this simulation.
        OrderGenerator *order_gen = nullptr;
        // Dispatcher for the simulation.
        Dispatcher *dispatcher = nullptr;
        // Factory that the dispatcher will use.
        Factory factory;
        // The barrier.
        Barrier *barrier = nullptr;
};

#endif
