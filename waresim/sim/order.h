#ifndef SIM_ORDER_H
#define SIM_ORDER_H

/**
 * @author Joshua A. Campbell
 *
 * An order within the simulation.
 */
class Order
{
    public:
        Order();
        Order(int);
        bool operator==(const Order &) const;

        // Returns the position of this order in the factory.
        int get_pos();

    private:
        static int curr_id;
        // ID of this order.
        int my_id;
        // Location of this order.
        int pos;
};


#endif
