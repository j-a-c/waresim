#include "worker.h"

Worker::Worker(int pos)
{
    this->pos = pos;
}

Worker::~Worker()
{

}

void Worker::assign(Order order)
{
    this->orders.push_back(order);
}
