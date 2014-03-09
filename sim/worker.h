#ifndef SIM_WORKER_H
#define SIM_WORKER_H

#include <vector>

#include "order.h"

/**
 * Represents a worker within the factory.
 */
class Worker
{
    public:
        Worker(int);
        ~Worker();

        // Assign an order to the worker.
        void assign(Order order);

    private:
        // The worker's position within the factory.
        int pos;
        // Orders the worker has to fulfill.
        std::vector<Order> orders{};
};

#endif
