#ifndef WARESIM_WARESIM_ALGO_PATH_PATH_ALGO
#define WARESIM_WARESIM_ALGO_PATH_PATH_ALGO

#include <vector>

#include "../../constants.h"
#include "../../warehouse.h"

/**
 * Base class for pathfinding algorithms.
 */
class PathAlgo
{
    public:
        PathAlgo();
        virtual ~PathAlgo();

        // Finds a path from the start to end point in the given warehouse.
        virtual std::vector<int> find(Warehouse*, int, int) = 0;

    private:

};

#endif
