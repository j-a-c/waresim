#include "simulation.h"

/**
 * Driver for Waresim's simulation.
 */
int main()
{
    // Begin config
    int sim_length = 5;
    // End config

    Simulation sim(sim_length);

    sim.run();
    sim.join();

    return 0;
}
