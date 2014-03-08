#include "rand_dispatch_algo.h"

// TODO Delete later, used for debug.
#include <iostream>

/**
 * Default constructor.
 */
RandDispatchAlgo::RandDispatchAlgo()
{

}

/**
 * Constructor.
 *
 * @param Rand The Rand to use.
 */
RandDispatchAlgo::RandDispatchAlgo(Rand rand)
{
    this->rand = rand;
}

/**
 * Destructor.
 */
RandDispatchAlgo::~RandDispatchAlgo()
{

}

void RandDispatchAlgo::assign_order(Factory *factory, Order order)
{
    auto workers = factory->get_workers();

    int index = workers.size() * rand.rand();

    std::cout << "Assigned to worker # " << index << std::endl;
    
    // TODO Assign.
}
