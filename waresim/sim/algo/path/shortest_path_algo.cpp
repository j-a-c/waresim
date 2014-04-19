#include "shortest_path_algo.h"

#include <limits>

/**
 * Constructor.
 * Sets the random number generator used to select between paths of the same
 * length.
 */
ShortestPathAlgo::ShortestPathAlgo(Rand rand)
{
    this->rand = rand;
}

/**
 * Destructor.
 */
ShortestPathAlgo::~ShortestPathAlgo()
{

}

/**
 * Computes the shortest path between the start and end positions using
 * Dijsktra's algorithm. Assumes all points are reachable within the warehouse.
 * If two nodes have the same distance, we flip a coin to decide which one we
 * will choose as the parent.
 *
 * TODO Implement using a faster data structure (currently uses a plain vector).
 *
 * @param start The starting position.
 * @param end The ending position.
 */
std::vector<int> ShortestPathAlgo::find(Warehouse* warehouse, int start, int end)
{
    // The path we are going to return.
    std::vector<int> path{};

    // We define the max distance to a little less that half the max integer
    // representable because we do not want to run into overflow problems.
    int MAX = (std::numeric_limits<int>::max()/ 2) - 2;
    // Used to mark paths we have not found yet.
    int UNDEFINED = -1;

    // Create a copy of the layout to work with.
    auto layout = warehouse->get_layout();

    // The vertices we have checked.
    std::vector<bool> checked(layout.size(), false);

    // Initialize distance values.
    std::vector<int> dists(layout.size(), MAX);
    dists[start] = 0;

    // Holds the previous nodes so we can calculate the path.
    std::vector<int> previous(layout.size(), UNDEFINED);

    // Get the warehouse dimensions.
    int width = warehouse->get_width();
    int height = warehouse->get_height();

    // We will loop until we find the shortest path to 'end'.
    // We are assuming all points are reachable in the warehouse with this while
    // loop condition.
    while(true)
    {
        // Find the vertex with the smallest distance that we have not yet 
        // checked.
        int curr_index = 0;
        int min_index = -1;
        int min_dist = MAX;
        for (auto &dist : dists)
        {
            if (dist < min_dist && !checked[curr_index])
            {
                min_index = curr_index;
            }
            curr_index++;
        }

        // Break if we have found the vertex we were looking for.
        if (min_index == end)
            break;

        // Mark this node as checked.
        checked[min_index] = true;

        /*
         * Check all neighbors. The neighbor must exist, must not already be
         * checked, and must be either and empty location or the end node.
         * Furthermore, we do not allow bin-to-bin travel or drop-to-drop 
         * travel.
         */

        // TODO Consider WORKER_LOC nodes?

        // The distance from the current vertex to its elligible neighbors.
        int new_dist = dists[min_index] + 1;

        std::vector<int> neighbors{};
        // Determine this node's valid neighbors.

        // Top neighbor.
        if ((min_index / height) != (height-1))
            neighbors.push_back(min_index + width);
        // Bottom neighbor.
        if (min_index >= width)
            neighbors.push_back(min_index - width);
        // Left neighbor.
        if ((min_index % width) != 0)
            neighbors.push_back(min_index - 1);
        // Right neighbor.
        if (((min_index+1) % width) != 0)
            neighbors.push_back(min_index + 1);

        for (auto &neigh_index : neighbors)
        {
            // Check if neighbor is valid.
            if ( !checked[neigh_index] && 
                    ((layout[neigh_index] == EMPTY_LOC) || (neigh_index == end)) &&
                    (layout[min_index] != BIN_LOC || layout[neigh_index] != BIN_LOC) &&
                    (layout[min_index] != DROP_LOC || layout[neigh_index] != DROP_LOC)
               )
            {
                // Check if this distance is shorter than the previously found
                // distance. Update the min distance and the parent.
                if (new_dist < dists[neigh_index])
                {
                    dists[neigh_index] = new_dist;
                    previous[neigh_index] = min_index;
                }
                // If this distance is equal to the previous one, we flip a
                // coin to decide whether or not to replace the old parent with
                // the new one.
                else if (new_dist == dists[neigh_index])
                {
                    if (rand.rand() > 0.5)
                        previous[neigh_index] = min_index;
                }
            }
        }
    }

    // Calculate the path. The path will not include the start index.
    int curr_index = end;
    while (previous[curr_index] != UNDEFINED)
    {
        path.insert(path.begin(), curr_index);
        curr_index = previous[curr_index];
    }

    /* Debug statement.
    std::string log_msg{"Path found: ("};
    log_msg.append(std::to_string(start));
    log_msg.append(",");
    log_msg.append(std::to_string(end));
    if (path.size() > 0)
        log_msg.append(")\n");
    for (auto &step : path)
    {
        log_msg.append("\t");
        log_msg.append(std::to_string(step));
    }
    logger.log(log_msg);
    */

    return path;
}

