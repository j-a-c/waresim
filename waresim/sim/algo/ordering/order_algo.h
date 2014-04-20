#ifndef WARESIM_SIM_ALGO_ORDERING_ORDER_ALGO
#define WARESIM_SIM_ALGO_ORDERING_ORDER_ALGO

#include "../../order.h"
#include "../../warehouse.h"

/**
 * Defines an interface for order generating algorithms.
 * The idea is that there are two steps to generating an order: deciding
 * whether or not an order should be generated, and then generating the actual
 * order. In client code, it is expected that order_ready() will be called
 * before get_new_order().
 */
class OrderAlgo
{
    public:
        OrderAlgo();
        virtual ~OrderAlgo();

        // Returns true if there is an order ready to be fulfilled.
        virtual bool order_ready() = 0;
        // Returns the new order to be fulfilled.
        virtual Order get_new_order(Warehouse *) = 0;

    private:
};

#endif
