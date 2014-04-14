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

void RandDispatchAlgo::assign_order(Warehouse *warehouse, Order order)
{
    std::vector<Worker>& workers = warehouse->get_workers();

    int index = workers.size() * rand.rand();

    std::cout << "Assigned to worker # " << workers[index].get_id() << std::endl;
    
    // Assign job to the worker.
    Worker& worker = workers[index];
    worker.assign(order);
}
