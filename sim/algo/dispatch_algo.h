#ifndef SIM_ALGO_DISPATCH_ALGO_H
#define SIM_ALGO_DISPATCH_ALGO_H

#include "../factory.h"
#include "../order.h"

/**
 * Encapsulates an algorithm that determines which worker gets which order.
 */
class DispatchAlgo
{
    public:
        virtual ~DispatchAlgo();
        // Assigns the order to one of the workers in the factory.
        virtual void assign_order(Factory *, Order) = 0;
    private:

};

#endif
