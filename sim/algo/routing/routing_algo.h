#ifndef SIM_ALGO_ROUTING_ROUTING_ALGO_H
#define SIM_ALGO_ROUTING_ROUTING_ALGO_H

#include "../../factory.h"
#include "../../worker.h"

/**
 * Encapsulates an algorithm that determines which the order in which workers
 * process their orders.
 */
class RoutingAlgo
{
    public:
        virtual ~RoutingAlgo();
        // Route the worker.
        virtual void route_worker(Factory *, Worker) = 0;
};

#endif
