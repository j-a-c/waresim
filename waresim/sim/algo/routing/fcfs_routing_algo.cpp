#include "fcfs_routing_algo.h"

// TODO Delete later, used for debug.
#include <iostream>

/**
 * Routes the specified worker.
 *
 * @param warehouse The warehouse for the simulation.
 * @param worker A reference to the worker to route.
 */
void FCFSRoutingAlgo::route_worker(Warehouse *warehouse, Worker& worker)
{

    // Get the orders this worker has fulfilled.
    std::vector<Order> orders_ready = worker.get_ready_orders();

    // Get the orders for this worker.
    std::vector<Order> orders = worker.get_orders();

    // This is the logic for determining when a worker is ready to be routed
    // back to a drop off location.
    if (orders_ready.size() > 2 || (orders.empty() && orders_ready.size() > 0))
    {
        // This is the logic for determining which location a worker will drop
        // off its fulfilled orders to. We will simply choose the first one for
        // ease.
        int drop_loc = warehouse->get_drops()[0];
        worker.set_routed(true);
        worker.set_current_dest(drop_loc);
        std::cout << "Routing to drop at: " << drop_loc << std::endl;
        return;
    }
 
    // If there are orders to fulfill, we will route this worker towards the
    // oldest one (FCFS).
    if (!orders.empty())
    {
        std::cout << "Routing to " << orders[0].get_pos() << std::endl;
        // Set the current order for the worker.
        worker.set_current_order(orders[0]);
        // Set the current destination for the worker.
        worker.set_current_dest(orders[0].get_pos());
        // Remove the order from the worker since we are going to fulfill it.
        worker.remove_order(orders[0]);
        // This worker has been routed.
        worker.set_routed(true);
        return;
    }
    
    // At this point the worker has nothing to do, so we will route it back to
    // its initial location.
    std::cout << "Routing to initial pos at: " << worker.get_initial_pos() << std::endl;
    worker.set_current_dest(worker.get_initial_pos());
    worker.set_routed(true);
    return;
    
}
