#include <algorithm>
#include <ctime>
#include <limits>

#include "constants.h"
#include "scheduler.h"

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
 *
 * @param i The upper bound for the randomly generated integer.
 */
int Scheduler::rand_int(int i)
{
    return rand.rand() * i;  
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

    // Debug statement.
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

    return path;
}

/**
 * Private method that encapsulates the scheduling algorithm so it can be run in
 * a thread.
 */
void Scheduler::run()
{
    // Will be used for logging purposes.
    std::string log_msg;

    // Continue simulation until sim_length seconds have elapsed.
    while (difftime(time(nullptr), start_time) < sim_length)
    {
        // Get the warehouse workers.
        std::vector<Worker>& workers = warehouse->get_workers();

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
        
        // Get the warehouse layout.
        auto layout = warehouse->get_layout();

        // We now have a random order in which to process the workers. We
        // will process each worker once. Processing means moving (or deciding
        // not to move) each worker one unit in the warehouse.
        for (auto &index : sched_order)
        {
            Worker& worker = workers[index]; 

            // Log message.
            log_msg = std::string{"Scheduling worker: "};
            log_msg.append(std::to_string(worker.get_id()));
            logger.log(log_msg);

            // If worker is not on a path, try apply routing policy.
            if (!worker.is_routed())
            {
                // Log message.
                log_msg = std::string{"Worker "};
                log_msg.append(std::to_string(worker.get_id()));
                log_msg.append(" is NOT routed.");
                logger.log(log_msg);

                // Route the worker.
                routing_algo->route_worker(warehouse, worker);
            }
            else
            {
                // Log message.
                log_msg = std::string{"Worker "};
                log_msg.append(std::to_string(worker.get_id()));
                log_msg.append(" is routed already.");
            }

            // If worker does not have a path, calculate one. 
            if (worker.get_path().empty())
            {
                worker.set_path( 
                        shortest_path(worker.get_pos(), 
                            worker.get_current_dest()));
            }

            // Get the path for the worker we are scheduling.
            auto path = worker.get_path();

            // Check if path is empty - that means this worker was
            // assigned to a bin it is currently on!
            if (path.empty())
            {
                worker.set_routed(false);

                // Update to 'drop off ready' if we reached a bin location.
                if (layout[worker.get_pos()] == BIN_LOC)
                {
                    worker.reached_order();
                    worker.set_drop_status(true);
                }
                // Update to 'not drop off ready' if we reached a drop off
                // location.
                if (layout[worker.get_pos()] == DROP_LOC)
                {
                    worker.drop_off();
                    worker.set_drop_status(false);
                }

                continue;
            }

            // Get the next position.
            int next_pos = path[0];
            // Get the old position.
            int curr_pos = worker.get_pos(); 

            /*
             * Check for collisions.
             */

            int height = warehouse->get_height();
            int width = warehouse->get_width();

            // Marks whether we can move in this position, if the position
            // we want to move the worker in is already occupied AND if the
            // worker has a smaller ID than the other worker.
            bool right_valid = false;
            bool left_valid = false;
            bool top_valid = false;
            bool bot_valid = false;

            // The worker occupying the spot we want to move.
            // We set this to the curent worker for convenience because we
            // did not specify a Worker() constructor.
            Worker next_pos_worker = worker;

            // Top neighbor.
            if (((curr_pos / height) != (height-1)) && (layout[curr_pos+width] == EMPTY_LOC))
                top_valid = true;
            // Bottom neighbor.
            if ((curr_pos >= width) && (layout[curr_pos-width] == EMPTY_LOC))
                bot_valid = true;
            // Left neighbor.
            if (((curr_pos % width) != 0) && (layout[curr_pos-1] == EMPTY_LOC))
                left_valid = true;
            // Right neighbor.
            if ((((curr_pos+1) % width) != 0) && (layout[curr_pos+1] == EMPTY_LOC))
                right_valid = true;

            // Is the next position for this worker taken?
            int next_pos_taken = false;

            // Get the current worker positions.
            // We determined which position we could move in if there is a
            // contention for our next spot, but now we also want to check
            // that these position are not occupied themselves.
            // We will also check that the next position for this worker is
            // empty.
            for (auto it = workers.begin(); it != workers.end(); ++it)
            {
                int other_pos = (*it).get_pos();

                // This set of statements will determine which of neighboring
                // positions currently have a worker on them.
                if (top_valid && (other_pos == curr_pos+width))
                {
                    top_valid = false;
                }
                else if (bot_valid && (other_pos == curr_pos-width))
                {
                    bot_valid = false; 
                }
                else if (left_valid && (other_pos == curr_pos-1))
                {
                    left_valid = false;
                }
                else if (right_valid && (other_pos == curr_pos+1))
                {
                    right_valid = false;
                }

                // This statement will check if the position we intend to move
                // to next already has a worker on it.
                if (next_pos == other_pos)
                {
                    next_pos_taken = true;
                    next_pos_worker = *it;
                }
            }

            // Warehouse position is already occupied.
            if (next_pos_taken)
            {
                // Log some details.
                log_msg = std::string{"Worker # "};
                log_msg.append(std::to_string(index));
                log_msg.append("'s next position is taken.");
                logger.log(log_msg);

                // Worker with the smaller ID (S) "backs off" from the worker with
                // the larger ID (L).
                if (worker.get_id() > next_pos_worker.get_id())
                {
                    // Log some details.
                    log_msg = std::string{"Worker # "};
                    log_msg.append(std::to_string(index)); 
                    log_msg.append(" will NOT back off.");
                    logger.log(log_msg);

                    warehouse->move_worker(curr_pos, curr_pos);
                    // We mark the spot the worker attempted to move as a
                    // contention spot.
                    warehouse->mark_contention(next_pos);
                    continue;
                }

                // Log some details.
                log_msg = std::string{"Worker # "};
                log_msg.append(std::to_string(index));
                log_msg.append(" WILL back off.");
                logger.log(log_msg);

                // S tries left, right, back, up, in that order. Move and
                // insert old position to front of the current path.
                if (left_valid)
                {
                    next_pos = curr_pos - 1;
                    // Set the worker's current position.
                    worker.set_pos(next_pos);
                    // Update and set the path.
                    path.insert(path.begin(), curr_pos);
                    worker.set_path(path);
                    // Update the warehouse layout.
                    warehouse->move_worker(curr_pos, next_pos);

                    // Log the next position.
                    log_msg = std::string("Moving worker # ");
                    log_msg.append(std::to_string(index));
                    log_msg.append(" to position: ");
                    log_msg.append(std::to_string(next_pos));
                }
                else if (right_valid)
                {
                    next_pos = curr_pos + 1;
                    // Set the worker's current position.
                    worker.set_pos(next_pos);
                    // Update and set the path.
                    path.insert(path.begin(), curr_pos);
                    worker.set_path(path);
                    // Update the warehouse layout.
                    warehouse->move_worker(curr_pos, next_pos);
                    
                    // Log the next position.
                    log_msg = std::string("Moving worker # ");
                    log_msg.append(std::to_string(index));
                    log_msg.append(" to position: ");
                    log_msg.append(std::to_string(next_pos));

                }
                else if (bot_valid)
                {
                    next_pos = curr_pos - width;
                    // Set the worker's current position.
                    worker.set_pos(next_pos);
                    // Update and set the path.
                    path.insert(path.begin(), curr_pos);
                    worker.set_path(path);
                    // Update the warehouse layout.
                    warehouse->move_worker(curr_pos, next_pos);

                    // Log the next position.
                    log_msg = std::string("Moving worker # ");
                    log_msg.append(std::to_string(index));
                    log_msg.append(" to position: ");
                    log_msg.append(std::to_string(next_pos));
                }
                else if (top_valid)
                {
                    next_pos = curr_pos + width;
                    // Set the worker's current position.
                    worker.set_pos(next_pos);
                    // Update and set the path.
                    path.insert(path.begin(), curr_pos);
                    worker.set_path(path);
                    // Update the warehouse layout.
                    warehouse->move_worker(curr_pos, next_pos);

                    // Log the next position.
                    log_msg = std::string("Moving worker # ");
                    log_msg.append(std::to_string(index));
                    log_msg.append(" to position: ");
                    log_msg.append(std::to_string(next_pos));
                }
                else
                {
                    // We have experience a deadlock because of the scheduling
                    // protocol. This means that the scheduling protocol is not
                    // 'correct'.

                    // Log a warning.
                    log_msg = std::string{"Deadlock found!"};
                    logger.log(log_msg, LogLevel::Warning);
                    
                    // We will not move this turn.
                    warehouse->move_worker(curr_pos, curr_pos);
                    warehouse->mark_deadlock(curr_pos);
                }
            }
            else // Warehouse position is not occupied.
            { 
                // Set the worker's current position.
                worker.set_pos(next_pos);
                // Update and set the path.
                path.erase(path.begin());
                worker.set_path(path);
                // Update the warehouse layout.
                warehouse->move_worker(curr_pos, next_pos);

                std::cout << "Moving worker # " << index << " to position: " 
                    << next_pos << std::endl;
            }

            // Update to 'not routed' if we have reached destination.
            if (path.empty())
            {
                worker.set_routed(false);

                // Update to 'drop off ready' if we reached a bin location.
                if (warehouse->get_layout()[next_pos] == BIN_LOC)
                {
                    worker.reached_order();
                    worker.set_drop_status(true);
                }
                // Update to 'not drop off ready' if we reached a drop off
                // location.
                if (warehouse->get_layout()[next_pos] == DROP_LOC)
                {
                    worker.drop_off();
                    worker.set_drop_status(false);
                }
            }

        }// End worker for loop

        // Tell that warehouse that we have finished processing all workers.
        warehouse->update_iteration();

        barrier->arrive();
    }// End while loop
}

/**
 * Assign the warehouse used in the simulation.
 *
 * @param warehouse The warehouse to use.
 */
void Scheduler::set_warehouse(Warehouse *warehouse)
{
    this->warehouse = warehouse;
}


/**
 * Set the barrier to synchronize on.
 *
 * @param barrier The barrier to synchronize on.
 */
void Scheduler::set_barrier(Barrier *barrier)
{
    this->barrier = barrier;
}

/**
 * Set the random number generator.
 *
 * @param rand The random number generator.
 */
void Scheduler::set_rand(Rand rand)
{
    this->rand = rand;
}

/**
 * Set the log file directory.
 *
 * @param dir The new log file directory.
 */
void Scheduler::set_log_dir(std::string dir)
{
    logger.set_up(dir);
}

/**
 * Set the routing algorithm.
 *
 * @param routing_algo The routing algorithm to use.
 */
void Scheduler::set_algo(RoutingAlgo *routing_algo)
{
    this->routing_algo = routing_algo;
}
