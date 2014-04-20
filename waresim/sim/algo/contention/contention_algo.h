#ifndef WARESIM_SIM_ALGO_CONTENTION_CONTENTION_ALGO_H
#define WARESIM_SIM_ALGO_CONTENTION_CONTENTION_ALGO_H

#include "../../warehouse.h"
#include "../../worker.h"
#include "../../../logger/logger.h"

/**
 * Interface for contention algorithms. A contention algorithm is used if a
 * worker is trying to move to a spot that is already occupied.
 *
 * Note that the contention algorithm is responsible for updating the worker's
 * movements within the warehouse!
 */
class ContentionAlgo
{
    public:
        ContentionAlgo();
        virtual ~ContentionAlgo();

        // Called when a contention must be handle.
        virtual void handle(Worker&, Worker&, Warehouse *,
                bool, bool, bool, bool, Logger *) = 0;
    private:
};

#endif
