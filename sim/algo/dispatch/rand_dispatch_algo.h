#ifndef SIM_ALGO_DISPATCH_RAND_DISPATCH_ALGO_H
#define SIM_ALGO_DISPATCH_RAND_DISPATCH_ALGO_H

#include "dispatch_algo.h"
#include "../../rand/rand.h"

/**
 * Encapsulates an algorithm that randomly selects a worker in the factory to
 * process the order.
 */
class RandDispatchAlgo : public DispatchAlgo
{
    public:
        RandDispatchAlgo();
        RandDispatchAlgo(Rand);
        ~RandDispatchAlgo();

        void assign_order(Factory *, Order) override;
    private:
        Rand rand;
};

#endif
