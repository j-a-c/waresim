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

/**
 * Assigns the order to one of the workers in the warehouse. This
 * implementation selects a worker at random and assigns the order to it.
 *
 * @param warehouse The warehouse for the simulation.
 * @param order The order to assign.
 */
void RandDispatchAlgo::assign_order(Warehouse *warehouse, Order order)
{
    std::vector<Worker>& workers = warehouse->get_workers();

    // Select a random worker index.
    int index = workers.size() * rand.rand();

    std::cout << "Assigned to worker # " << workers[index].get_id() << std::endl;
    
    // Assign job to the worker.
    Worker& worker = workers[index];
    worker.assign(order);
}
