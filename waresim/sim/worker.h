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
        // Return the orders assigned to this worker.
        std::vector<Order> get_orders();
        // Return the orders the worker has picked up.
        std::vector<Order> get_ready_orders();
        // Get the current order.
        Order get_current_order();
        // Set the current order.
        void set_current_order(Order);
        // Called when the worker has reached its order.
        void reached_order();
        // Called when the worker reaches a drop off location.
        void drop_off();
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
        // Get the worker's initial position in the warehouse.
        int get_initial_pos();
        // Set the current destination for the worker.
        void set_current_dest(int);
        // Get the current destination for the worker.
        int get_current_dest();

    private:
        static int curr_id;

        // The worker's ID.
        int my_id;
        // The worker's initial position within the factory.
        int initial_pos;
        // The worker's position within the factory.
        int pos;
        // The worker's current destination.
        int dest;
        // Orders the worker has to fulfill.
        std::vector<Order> orders{};
        // Orders that are ready to be dropped off.
        std::vector<Order> orders_ready{};
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
