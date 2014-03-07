#ifndef SIM_DISPATCHER_H
#define SIM_DISPATCHER_H

#include <ctime>

#include "concurrent/barrier.h"
#include "factory.h"
#include "order_generator.h"
#include "simulator.h"

/**
 * @author Joshua A. Campbell
 *
 * Calculates the dispatching algorithm for the simulation.
 */
class Dispatcher : public Simulator
{
    public:
        Dispatcher(time_t, int);

        // Set the factory.
        void set_factory(Factory *);
        // Set the order generator.
        void set_order_generator(OrderGenerator *);
        // Set the barrier.
        void set_barrier(Barrier *);
    private:
        // Override from Simulator.
        void simulate() override;

        // The start time of the simulation.
        time_t start_time;
        // The length of the simulation in seconds.
        int sim_length;
        // The factory;
        Factory *factory = nullptr;
        // The order generator.
        OrderGenerator *order_gen = nullptr;
        // The barrier.
        Barrier *barrier = nullptr;
};

#endif
