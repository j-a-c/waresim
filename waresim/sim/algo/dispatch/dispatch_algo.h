#ifndef SIM_ALGO_DISPATCH_DISPATCH_ALGO_H
#define SIM_ALGO_DISPATCH_DISPATCH_ALGO_H

#include "../../order.h"
#include "../../warehouse.h"
#include "../../../logger/logger.h"

/**
 * Encapsulates an algorithm that determines which worker gets which order.
 */
class DispatchAlgo
{
    public:
        virtual ~DispatchAlgo();
        // Assigns the order to one of the workers in the warehouse.
        virtual int assign_order(Warehouse *, Order) = 0;
    private:

};

#endif
