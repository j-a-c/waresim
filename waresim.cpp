#include <string>

#include "sim/simulation.h"

/**
 * @author Joshua A. Campbell
 *
 * Driver for Waresim's simulation.
 */
int main()
{
    // TODO Create a config file?
    // Begin config
    int sim_length = 5;
    std::string factory_file = "factory.txt";
    // End config

    Simulation sim(sim_length, factory_file);

    sim.run();
    sim.join();

    return 0;
}
