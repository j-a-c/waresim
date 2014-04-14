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
        
        // Any initial set up needed by this view.
        virtual void setup() = 0;
        // Starts the rendering.
        virtual void run() = 0;
        // Set the warehouse to render.
        virtual void set_warehouse(Warehouse *) = 0;
        // Sets the length of the simulation to render.
        virtual void set_sim_params(time_t, int) = 0;
    private:
        // The warehouse to render.
        Warehouse *warehouse = nullptr;
        // The start time of the simulation.
        time_t start_time;
        // The length of the simulation.
        int sim_length;

};

#endif
