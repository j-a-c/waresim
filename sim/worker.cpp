#include <algorithm>

#include "worker.h"

int Worker::curr_id = 0;

Worker::Worker(int pos) : my_id(curr_id++)
{
    this->pos = pos;
    this-> initial_pos = pos;
}

Worker::~Worker()
{

}

/**
 * == operator. Checks that the ids of the two workers are the same.
 */
bool Worker::operator==(const Worker &other) const
{
    return my_id == other.my_id;
}

/**
 * Assigns a new order to this worker.
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
    auto pos = std::find(orders.begin(), orders.end(), order);
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
 * Clears tje list of orders the worker is holding.
 */
void Worker::drop_off()
{
    orders_ready.clear();
}

/**
 * Returns true if this worker is routed.
 */
bool Worker::is_routed()
{
    return routed;
}

/**
 * Returns the orders assigned to this vector.
 */
std::vector<Order> Worker::get_orders()
{
    return orders;
}

/**
 * Returns the order the worker is currently fulfilling.
 */
Order Worker::get_current_order()
{
    return current_order;
}

/**
 * Sets the current order the worker is currently fulfilling.
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
 */
int Worker::get_current_dest()
{
    return dest;
}

/**
 * Set the drop status of this worker.
 */
void Worker::set_drop_status(bool b)
{
    drop_needed = b;
}

/**
 * Returns true if the Worker has orders it needs to drop off.
 */
bool Worker::get_drop_status()
{
    return drop_needed;
}

/**
 * Returns the position of the Worker within the factory.
 */
int Worker::get_pos()
{
    return pos;
}

/**
 * Sets the position of the Worker within the factory.
 */
void Worker::set_pos(int new_pos)
{
    this->pos = new_pos;
}

/**
 * Returns the path the worker is currently following.
 */
std::vector<int> Worker::get_path()
{
    return path;
}

/**
 * Sets the path the worker is currently following.
 */
void Worker::set_path(std::vector<int> new_path)
{
    this->path = new_path;
}

/**
 * Returns the worker's ID.
 */
int Worker::get_id()
{
    return my_id;
}

std::vector<Order> Worker::get_ready_orders()
{
    return orders_ready;
}

/**
 * Returns the initial position of the worker in the factory.
 */
int Worker::get_initial_pos()
{
    return initial_pos;
}
