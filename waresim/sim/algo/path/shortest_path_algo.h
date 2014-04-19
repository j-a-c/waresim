#ifndef WARESIM_WARESIM_SIM_ALGO_SHORTEST_PATH_ALGO
#define WARESIM_WARESIM_SIM_ALGO_SHORTEST_PATH_ALGO

#include "path_algo.h"

#include "../../rand/rand.h"

class ShortestPathAlgo : public PathAlgo
{
    public:
        ShortestPathAlgo(Rand);
        ~ShortestPathAlgo();

        // Finds the shortest path between the start and end point using the
        // shortest path algorithm.
        std::vector<int> find(Warehouse*, int, int) override;

    private:
        Rand rand;
};

#endif
