#ifndef WARESIM_SIM_ALGO_CONTENTION_BACKOFF_ALGO_H
#define WARESIM_SIM_ALGO_CONTENTION_BACKOFF_ALGO_H

/*
 * Interface of a contention algorithm. This algorithm uses the worker's
 * id's to impose an ordering on the workers. The worker will the smaller id
 * will "back off" from the worker with the larger id.
 */
#include "contention_algo.h"

class BackoffAlgo : public ContentionAlgo
{
    public:
        BackoffAlgo();
        ~BackoffAlgo();
        
        // Handles the contention.
        void handle(Worker&, Worker&, Warehouse*,
                bool, bool, bool, bool, Logger *);

    private:
        // Used for logging.
        std::string log_msg;
};

#endif
