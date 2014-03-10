#include "fcfs_routing_algo.h"

// TODO Delete later, used for debug.
#include <iostream>

void FCFSRoutingAlgo::route_worker(Factory *factory, Worker worker)
{
    // Get the orders for this worker.
    std::vector<Order> orders = worker.get_orders();
    
    // If there are orders to fulfill, we will route this worker towards the
    // oldest one (FCFS).
    if (!orders.empty())
    {
        std::cout << "Routing to " << orders[0].get_pos() << std::endl;
        // Set the current order.
        worker.set_current_order(orders[0]);
        // Remove the order since we are going to fulfill it now.
        worker.remove_order(orders[0]);
        // This worker has been routed.
        worker.set_routed(true);
    }
}
