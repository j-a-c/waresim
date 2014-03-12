#include <algorithm>
#include <ctime>
#include <limits>

#include "constants.h"
#include "scheduler.h"

// TODO Delete later, used for debug.
#include <iostream>

/**
 * Constructor.
 *
 * @param start_time
 * @param sim_length
 */
Scheduler::Scheduler(time_t start_time, int sim_length)
{
    this->start_time = start_time;
    this->sim_length = sim_length;
}

/**
 * Returns a random integer in the rand [0,i).
 */
int Scheduler::rand_int(int i)
{
    return rand.rand() * i;  
}

/**
 * Computes the shortest path between the start and end positions using
 * Dijsktra's algorithm. Assumes all points are reachable within the factory.
 *
 * TODO Implement using a faster data structure (currently uses a plain vector).
 *
 * @param start The starting position.
 * @param end The ending position.
 */
std::vector<int> Scheduler::shortest_path(int start, int end)
{
    // The path we are going to return.
    std::vector<int> path{};

    // We define the max distance to a little less that half the max integer
    // representable because we do not want to run into overflow problems.
    int MAX = (std::numeric_limits<int>::max()/ 2) - 2;
    // Used to mark paths we have not found yet.
    int UNDEFINED = -1;

    // Create a copy of the layout to work with.
    auto layout = factory->get_layout();

    // The vertices we have checked.
    std::vector<bool> checked(layout.size(), false);

    // Initialize distance values.
    std::vector<int> dists(layout.size(), MAX);
    dists[start] = 0;

    // Holds the previous nodes so we can calculate the path.
    std::vector<int> previous(layout.size(), UNDEFINED);

    // Get the factory dimensions.
    int width = factory->get_width();
    int height = factory->get_height();

    // We will loop until we find the shortest path to 'end'.
    // We are assuming all points are reachable in the factory with this while
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
         */

        // TODO Consider WORKER_LOC and empty nodes?

        // The distance from the current vertex to its elligible neighbors.
        int new_dist = dists[min_index] + 1;

        // Top neighbor.
        int neigh_index = min_index + width;
        if ( ((min_index / height) != (height-1)) && !checked[neigh_index] && 
                ((layout[neigh_index] == EMPTY_LOC) || (neigh_index == end)) )
        {
            if (new_dist < dists[neigh_index])
            {
                dists[neigh_index] = new_dist;
                previous[neigh_index] = min_index;
            }
        }
        // Bottom neighbor.
        neigh_index = min_index - width;
        if ( (min_index >= width) && !checked[neigh_index] &&
                ((layout[neigh_index] == EMPTY_LOC) || (neigh_index == end)) )
        {
            if (new_dist < dists[neigh_index])
            {
                dists[neigh_index] = new_dist;
                previous[neigh_index] = min_index;
            }
        }
        // Left neighbor.
        neigh_index = min_index - 1;
        if ( ((min_index % width) != 0) && !checked[neigh_index] && 
                ((layout[neigh_index] == EMPTY_LOC) || (neigh_index == end)) )
        {
            if (new_dist < dists[neigh_index])
            {
                dists[neigh_index] = new_dist;
                previous[neigh_index] = min_index;
            }
        }
        // Right neighbor.
        neigh_index = min_index + 1;
        if ( (((min_index+1) % width) != 0) && !checked[neigh_index] && 
                ((layout[neigh_index] == EMPTY_LOC) || (neigh_index == end)) )
        {
            if (new_dist < dists[neigh_index])
            {
                dists[neigh_index] = new_dist;
                previous[neigh_index] = min_index;
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
    
    std::cout << "Path found: (" << start << "," << end << ")" << std::endl;
    for (auto &step : path)
        std::cout << "\t" << step << std::endl;

    return path;
}

/**
 * Private method that encapsulates the scheduling algorithm so it can be run in
 * a thread.
 */
void Scheduler::run()
{
    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(time(nullptr), start_time) < sim_length)
    {
        // Get the factory workers.
        std::vector<Worker>& workers = factory->get_workers();

        // Create a randomized scheduling order. This simulates a bunch of
        // multithreaded workers.
        std::vector<int> sched_order{};
        int i = 0;
        for (auto &worker : workers)
        {
            sched_order.push_back(i);
            i++;
        }
        // Randomly shuffle the order.
        std::shuffle(sched_order.begin(), sched_order.end(), rand.get_urng());
    
        // We now have a random order in which to process the workers. Will
        // will process each worker once. Processing means moving (or deciding
        // not to move) each worker one unit in the factory.
        for (auto &index : sched_order)
        {
            Worker& worker = workers[index]; 

            std::cout << "Scheduling worker: " << worker.get_id() << std::endl;

            // If worker is not on a path, try apply routing policy.
            if (!worker.is_routed())
            {
                std::cout << "Worker " << worker.get_id() << " is NOT routed." << std::endl;
                routing_algo->route_worker(factory, worker);
            }
            else
            {
                std::cout << "Worker " << worker.get_id() << " is routed already." << std::endl;
            }

            // If worker still isn't routed, we need to either drop off an
            // order or wait somewhere.
            if (!worker.is_routed())
            {
                // TODO 
                std::cout << "Worker " << worker.get_id() << " is STILL NOT routed." << std::endl;
                if (worker.get_drop_status())
                {
                    // Proceed to drop location
                    // TODO
                }
                else
                {
                    // Go to inital position.
                    // TODO
                }

            }
            // Find a way to continue the path.
            else
            {
                // If worker does not have a path, calculate one. 
                if (worker.get_path().empty())
                {
                    worker.set_path( 
                            shortest_path(worker.get_pos(), 
                                worker.get_current_order().get_pos()));
                }

                // TODO Check for collisions.

                // Get the old position.
                int old_pos = worker.get_pos();
                // Get the path.
                auto path = worker.get_path();
                // Get the next position.
                int next_pos = path[0];
                // Set the worker's current position.
                worker.set_pos(next_pos);
                // Update and set the path.
                path.erase(path.begin());
                worker.set_path(path);
                // Update the factory layout.
                factory->move_worker(old_pos, next_pos);

                std::cout << "Moving worker # " << index << " to position: " 
                    << next_pos << std::endl;

                // Update to 'not routed' if we have reached destination.
                if (path.empty())
                {
                    worker.set_routed(false);

                    // Update to 'drop off ready' if we reached a bin location.
                    if (factory->get_layout()[next_pos] == BIN_LOC)
                        worker.set_drop_status(true);
                    // Update to 'not drop off ready' if we reached a drop off
                    // location.
                    if (factory->get_layout()[next_pos] == DROP_LOC)
                        worker.set_drop_status(false);
                }
            }
        }
        
        barrier->arrive();
    }
}

/**
 * Assign the factory used in the simulation.
 *
 * @param factory The factory to use.
 */
void Scheduler::set_factory(Factory *factory)
{
    this->factory = factory;
}


/**
 * Set the barrier to synchronize on.
 */
void Scheduler::set_barrier(Barrier *barrier)
{
    this->barrier = barrier;
}

/**
 * Set the random number generator.
 */
void Scheduler::set_rand(Rand rand)
{
    this->rand = rand;
}

/**
 * Set the routing algorithm.
 */
void Scheduler::set_algo(RoutingAlgo *routing_algo)
{
    this->routing_algo = routing_algo;
}
