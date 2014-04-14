#include "order.h"

int Order::curr_id = 0;

/**
 * Default constructor.
 */
Order::Order()
{

}

/**
 * Constructor. Will set the order's pos and give it a unique id.
 *
 * @param pos The position to pick this up from. The position should be in the
 * warehouse's coordinates.
 * 
 */
Order::Order(int pos) : my_id(curr_id++)
{
    this->pos = pos;
}

/**
 * == operator. Checks that the ids of the two Orders are the same.
 */
bool Order::operator==(const Order &other) const
{
    return my_id == other.my_id;
}

/**
 * Returns the position to pick this order up from. The position is in the
 * warehouse's coordinates.
 *
 * @return The position to pick this order up from.
 */
int Order::get_pos()
{
    return pos;
}
