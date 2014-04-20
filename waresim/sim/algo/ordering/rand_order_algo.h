#ifndef WARESIM_SIM_ALGO_ORDERING_RAND_ORDER_ALGO_H
#define WARESIM_SIM_ALGO_ORDERING_RAND_ORDER_ALGO_H

#include "order_algo.h"

#include "../../rand/rand.h"

/**
 * An implementation of OrderAlgo that has a 50% chance of generating a new
 * order each iteration and that selects a random bin if the order is
 * generated.
 */
class RandOrderAlgo : public OrderAlgo
{
    public:
        RandOrderAlgo(Rand);
        ~RandOrderAlgo();

        // Returns true if there is an order ready to be fulfilled.
        bool order_ready() override;
        // Returns the new order to be fulfilled.
        Order get_new_order(Warehouse *) override;

    private:
        // The random number generator to use.
        Rand rand;
};

#endif
