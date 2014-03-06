#ifndef SIM_SIMULATOR_H
#define SIM_SIMULATOR_H

#include <thread>

/**
 * @author Joshua A. Campbell
 *
 * Defines a object that is part of a simulation.
 */
class Simulator
{
    public:
        virtual ~Simulator();
        // Runs the simulation specified in simulate();
        void run();
        // Waits for the simulation to finish.
        void join();
    private:
        // This method will be run from the thread.
        virtual void simulate() = 0;
        // Thread to run simulate() from.
        std::thread thread;

    protected:
        // The minimum time in seconds between actions.
        // Adjust this to change the granularity of the simulations.
        const int TIME_STEP = 1;
};

#endif
