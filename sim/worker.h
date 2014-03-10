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
        bool operator==(const Worker &) const;

        // Assign an order to the worker.
        void assign(Order);
        // Remove an order from the worker.
        void remove_order(Order);
        // Returns true if the worker is routed.
        bool is_routed();
        // Set the worker's route status.
        void set_routed(bool);
        // Return the orders assigned to this vector.
        std::vector<Order> get_orders();
        // Get the current order.
        Order get_current_order();
        // Set the current order.
        void set_current_order(Order);
        // Set the drop status.
        void set_drop_status(bool);
        // Get the drop status.
        bool get_drop_status();
        // Get the position of this worker.
        int get_pos();
        // Set the position of this worker.
        void set_pos(int);
        // Get the worker's current path.
        std::vector<int> get_path();
        // Set the worker's current path.
        void set_path(std::vector<int>);
        // Get the worker's ID.
        int get_id();

    private:
        static int curr_id;

        // The worker's ID.
        int my_id;
        // The worker's initial position within the factory.
        int initial_pos;
        // The worker's position within the factory.
        int pos;
        // Orders the worker has to fulfill.
        std::vector<Order> orders{};
        // Is the worker routed?
        bool routed = false;
        // Does the worker need to drop-off orders?
        bool drop_needed = false;
        // The current order the worker is fulfilling.
        Order current_order;
        // The path the worker is following.
        std::vector<int> path{};
};

#endif