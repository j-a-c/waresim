#include <algorithm>

#include "worker.h"

// Used to assign a unique identifier to each worker.
int Worker::curr_id = 0;

/**
 * Constructor. Constructs a worker at the given initial position in the
 * warehouse. Also assigns a unique identifier to the worker.
 *
 * @param pos The initial position of the worker in the warehouse. The
 * assumption is that the initial position is the holding bay for the worker.
 * This position should be in the coordinates of the warehouse.
 */
Worker::Worker(int pos) : my_id(curr_id++)
{
    this->pos = pos;
    this-> initial_pos = pos;
}

/**
 * Destructor.
 */
Worker::~Worker()
{

}

/**
 * == operator. Checks that the ids of the two workers are the same.
 *
 * @return true is the two workers have the same ID.
 */
bool Worker::operator==(const Worker &other) const
{
    return my_id == other.my_id;
}

/**
 * Assigns a new order to this worker.
 *
 * @param order The new order to assign to the worker.
 */
void Worker::assign(Order order)
{
    this->orders.push_back(order);
}

/**
 * Removes the order from the list of orders this worker must process. Does not
 * do any range checking since we assume we are only removing orders we found
 * in this worker's list of orders.
 *
 * @param order The order to remove.
 */
void Worker::remove_order(Order order)
{
    // Find the order in the list of this worker's orders.
    auto pos = std::find(orders.begin(), orders.end(), order);
    // Erase the order.
    orders.erase(pos);
}

/**
 * Set this worker's routing status.
 *
 * @param b True if the worker has been assigned a new route, false otherwise.
 */
void Worker::set_routed(bool b)
{
    this->routed = b;
}

/**
 * Called when a worker reaches its order.
 * Sets its current order to the list of orders ready to be dropped off.
 */
void Worker::reached_order()
{
    orders_ready.push_back(current_order);
}

/**
 * Called when a worker reaches a drop off location. 
 * Clears the list of orders the worker is holding.
 */
void Worker::drop_off()
{
    orders_ready.clear();
}

/**
 * Returns true if this worker is routed. Being routed means that the worker
 * already has a path to sme destination assigned to it by the scheduler.
 *
 * @return true if the worker is routed.
 */
bool Worker::is_routed()
{
    return routed;
}

/**
 * Returns the orders assigned to this vector.
 *
 * @return The order assigned to the worker.
 */
std::vector<Order> Worker::get_orders()
{
    return orders;
}

/**
 * Returns the order the worker is currently fulfilling.
 *
 * @return The order that the worker is currently fulfilling.
 */
Order Worker::get_current_order()
{
    return current_order;
}

/**
 * Sets the current order the worker is currently fulfilling.
 *
 * @param o The order that the worker is currently fulfilling.
 */
void Worker::set_current_order(Order o)
{
    current_order = o;
}

/**
 * Sets the current destination for this workers.
 *
 * @param pos The current destination for this worker.
 */
void Worker::set_current_dest(int pos)
{
    this->dest = pos;
}

/**
 * Returns the current destination for this worker.
 *
 * @param The current destination for this worker.
 */
int Worker::get_current_dest()
{
    return dest;
}

/**
 * Set the drop status of this worker. A drop status of true means that the
 * worker needs to return to a drop location immediately before fulfilling
 * another order.
 *
 * @param b The drop status of this worker.
 */
void Worker::set_drop_status(bool b)
{
    drop_needed = b;
}

/**
 * Returns true if the Worker has orders it needs to drop off. If the drop
 * status is true, then the worker MUST return to a drop location before
 * fulfilling any other orders.
 *
 * @return true if the worker has orders that it needs to drop off.
 */
bool Worker::get_drop_status()
{
    return drop_needed;
}

/**
 * Returns the position of the Worker within the warehouse.
 *
 * @return The current position of the worker within the warehouse.
 */
int Worker::get_pos()
{
    return pos;
}

/**
 * Sets the position of the Worker within the warehouse.
 *
 * @param new_pos The new position of the warehouse within the warehouse.
 */
void Worker::set_pos(int new_pos)
{
    this->pos = new_pos;
}

/**
 * Returns the path the worker is currently following.
 *
 * @return The path the worker is currently following.
 */
std::vector<int> Worker::get_path()
{
    return path;
}

/**
 * Sets the path the worker is currently following.
 *
 * @param new_path The new path that the worker should follow.
 */
void Worker::set_path(std::vector<int> new_path)
{
    this->path = new_path;
}

/**
 * Returns the worker's ID. Each worker has a unique ID.
 *
 * @return The unique ID assigned to this worker.
 */
int Worker::get_id()
{
    return my_id;
}

/**
 * Returns the orders the worker is ready to drop off.
 *
 * @return The order the worker is ready to drop off.
 */
std::vector<Order> Worker::get_ready_orders()
{
    return orders_ready;
}

/**
 * Returns the initial position of the worker in the warehouse. It is assumed
 * that this initial position is the holding bay for the worker.
 *
 * @return The initial position of the worker in the warehouse.
 */
int Worker::get_initial_pos()
{
    return initial_pos;
}
