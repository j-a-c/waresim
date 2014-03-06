#ifndef SIM_DISPATCHER_H
#define SIM_DISPATCHER_H

#include <ctime>

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

        // Set the order generator.
        void set_order_generator(OrderGenerator *);
    private:
        // Override from Simulator.
        void simulate() override;

        // The start time of the simulation.
        time_t start_time;
        // The length of the simulation in seconds.
        int sim_length;
        // Thread for the Dispatcher to run on.
        std::thread dispatcher;
        // The order generator.
        OrderGenerator *order_gen;
};

#endif
