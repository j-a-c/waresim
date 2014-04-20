/*
 * Implementation for rand_order_algo.h
 */
#include "rand_order_algo.h"

/**
 * Constructor.
 *
 * @param rand The random number generator to use.
 */
RandOrderAlgo::RandOrderAlgo(Rand rand)
{
    this->rand = rand;
}

/**
 * Destructor.
 */
RandOrderAlgo::~RandOrderAlgo()
{

}

/**
 * Returns true if an order is ready, and false otherwise.
 */
bool RandOrderAlgo::order_ready()
{
        double d = rand.rand();
        
        if (d < 0.50)
            return true;
        else
            return false;
}

/**
 * Returns the new order to be fulfilled.
 *
 * @param w The warehouse to select from.
 */
Order RandOrderAlgo::get_new_order(Warehouse *w)
{
    // Get the location of all the bins in the warehouse.
    auto bins = w->get_bins();

    // Select a random bin index.
    int index = rand.rand() * bins.size();

    return Order(bins[index]);
}

