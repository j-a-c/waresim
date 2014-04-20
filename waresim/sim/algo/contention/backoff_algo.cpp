// Implementation of backoff_algo.h
#include "backoff_algo.h"

#include <string>

/**
 * Constructor.
 */
BackoffAlgo::BackoffAlgo()
{

}

/**
 * Destructor.
 */
BackoffAlgo::~BackoffAlgo()
{

}

/**
 * Handles the contention. The worker with the smaller ID will back off from
 * the worker with the larger ID.
 */
void BackoffAlgo::handle(Worker& worker, Worker& next_pos_worker,
        Warehouse *warehouse, bool top_valid, bool bot_valid, bool left_valid,
        bool right_valid, Logger * logger)
{
    /*
     * First, we need to set up some variables we will need for our
     * calculations.
     */

    int index = worker.get_id();
    // Get the path for the worker we are scheduling.
    auto path = worker.get_path();
    // Get the next position.
    int next_pos = path[0];
    // Get the old position.
    int curr_pos = worker.get_pos(); 
    // Get the warehouse dimensions.
    int width = warehouse->get_width();

    // Worker with the smaller ID (S) "backs off" from the worker with
    // the larger ID (L).
    if (worker.get_id() > next_pos_worker.get_id())
    {
        // Log some details.
        log_msg = std::string{"Worker # "};
        log_msg.append(std::to_string(index)); 
        log_msg.append(" will NOT back off.");
        logger->log(log_msg);

        warehouse->move_worker(curr_pos, curr_pos);
        // We mark the spot the worker attempted to move as a
        // contention spot.
        warehouse->mark_contention(next_pos);
        return;
    }

    // If we reached here, this worker has to back off.
    
    // Log some details.
    log_msg = std::string{"Worker # "};
    log_msg.append(std::to_string(index));
    log_msg.append(" WILL back off.");
    logger->log(log_msg);

    
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
        logger->log(log_msg, LogLevel::Warning);
        
        // We will not move this turn.
        warehouse->move_worker(curr_pos, curr_pos);
        warehouse->mark_deadlock(curr_pos);
        return;
    }

    logger->log(log_msg);

}
