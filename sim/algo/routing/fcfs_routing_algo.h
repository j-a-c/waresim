#ifndef SIM_ALGO_ROUTING_FCFS_ROUTING_ALGO_H
#define SIM_ALGO_ROUTING_FCFS_ROUTING_ALGO_H

#include "routing_algo.h"

/**
 * Encapsulates a first-come-first-serve routing algorithm for routing a worker
 * towards an order that needs to be fullfilled.
 */
class FCFSRoutingAlgo : public RoutingAlgo
{
    public:
        void route_worker(Factory *, Worker) override;
    private:
};

#endif
