#ifndef SIM_SCHEDULER_H
#define SIM_SCHEDULER_H

#include <ctime>

#include "concurrent/barrier.h"
#include "factory.h"
#include "simulator.h"

/**
 * Reponsible for scheduling the worker's movements in the factory.
 */
class Scheduler : public Simulator
{
    public:
        Scheduler(time_t, int);

        // Set the factory.
        void set_factory(Factory *);
        // Set the barrier.
        void set_barrier(Barrier *);

    private:
        // Override from Simulator
        void simulate() override;

        // The start time for the simulation.
        time_t start_time;
        // The length of the simulation.
        int sim_length;
        // The factory.
        Factory *factory = nullptr;
        // The barrier.
        Barrier *barrier = nullptr;
};

#endif
