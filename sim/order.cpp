#include "order.h"

int Order::curr_id = 0;

Order::Order()
{

}

Order::Order(int pos) : my_id(curr_id++)
{
    this->pos = pos;
}

bool Order::operator==(const Order &other) const
{
    return my_id == other.my_id;
}

int Order::get_pos()
{
    return pos;
}
