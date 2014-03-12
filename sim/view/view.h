#ifndef RENDER_VIEW_H
#define RENDER_VIEW_H

#include <ctime>

#include "../factory.h"
#include "../thread/thread.h"

/**
 * Renders the simulation.
 */
class View : public Thread
{

    public:
        virtual ~View() = 0;

        virtual void set_factory(Factory *) = 0;
        virtual void set_sim_params(time_t start_time, int sim_length) = 0;
    private:
        Factory *factory = nullptr;
        time_t start_time;
        int sim_length;

};

#endif
