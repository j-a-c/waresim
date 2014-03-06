#include "simulator.h"

Simulator::~Simulator(){}

void Simulator::run()
{
    thread = std::thread(&Simulator::simulate, this);
}

void Simulator::join()
{
    thread.join();
}

