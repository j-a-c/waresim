#ifndef RENDER_VIEW_H
#define RENDER_VIEW_H

#include <ctime>

#include "../warehouse.h"
#include "../thread/thread.h"

/**
 * Renders the simulation.
 */
class View //: public Thread
{

    public:
        virtual ~View();
        
        virtual void setup() = 0;
        virtual void run() = 0;
        virtual void set_warehouse(Warehouse *) = 0;
        virtual void set_sim_params(time_t, int) = 0;
    private:
        Warehouse *warehouse = nullptr;
        time_t start_time;
        int sim_length;

};

#endif
