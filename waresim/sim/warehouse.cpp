#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include "constants.h"
#include "warehouse.h"
#include "util/util.h"

// TODO Delete later, used for debug.
#include <iostream>

/**
 * Constructor.
 */
Warehouse::Warehouse()
{
    
}

/**
 * Destructor
 */
Warehouse::~Warehouse()
{

}

/**
 * Returns a vector containing the lines in a file.
 *
 * @param file The input file location.
 */
std::vector<std::string> Warehouse::read_file(std::string filename) 
{
    // Attempts to create stream.
    std::ifstream ifs(filename);

    if(!ifs.good()) throw("Cannot open file.");

    std::vector<std::string> lines;

    // Continue reading lines until we cannot read any more.
    for (;;) 
    {
        std::string line;
        std::getline(ifs, line);
        if(!ifs) break;
        lines.push_back(line);
    }

    // Check if we reached EOF before returning.
    if(!ifs.eof()) throw("Error reading file.");

    return lines;
}


/**
 * Returns the Warehouse formed from parsing the given file.
 *
 * @param warehouse_file The file to parse.
 */
Warehouse Warehouse::parse_default_warehouse(std::string warehouse_file)
{
    Warehouse warehouse;

    // Read warehouse layout file.
    auto lines = read_file(warehouse_file);

    // Set warehouse dimensions.
    warehouse.height = lines.size();
    warehouse.width = lines.at(0).length();

    // We will use this to hold warehouse.layout.
    std::vector<int> layout = 
        std::vector<int>(warehouse.height*warehouse.width, EMPTY_LOC);
    
    // Populate warehouse layout.
    for (int y = 0; y < warehouse.height; y++)
    {
        auto line = lines.at(y);

        for (int x = 0; x < warehouse.width; x++)
        {
            char marker = line.at(x);

            int pos = coord_to_pos(x, y, warehouse.width);

            // Depending on which marker we encounter, we will do a different a
            // initialization step for the warehouse. Markers are defined in
            // constants.h
            switch (marker)
            {
                case EMPTY_MARKER:
                    // We initialized everything to empty previously.
                    break;
                case BIN_MARKER:
                    layout[pos] = BIN_LOC;
                    // Push back a new bin location.
                    warehouse.bins.push_back(pos);
                    break;
                case DROP_MARKER:
                    layout[pos] = DROP_LOC;
                    // Push back a new drop location.
                    warehouse.drops.push_back(pos);
                    break;
                case WORKER_MARKER:
                    layout[pos] = WORKER_LOC;
                    // Push back a new worker at this location.
                    warehouse.worker_locs.push_back(pos);
                    warehouse.workers.push_back(Worker(pos));
                    break;
                case WALL_MARKER:
                    layout[pos] = WALL_LOC;
                    // Push back a new wall at this location.
                    warehouse.walls.push_back(pos);
                    break;
                default:
                    // If we find an invalid marker, we will ignore it and continue.
                    // We will print a small message though.
                    std::string msg{"Invalid marker at: ("};
                    msg += x;
                    msg += ",";
                    msg += y;
                    msg += ") : ";
                    msg += marker;
                    throw (msg);
            }
        }
    }

    // Set warehouse layout.
    warehouse.layout = layout;

    // Reserve memory for statistics.
    warehouse.heat_window.reserve(layout.size());
    warehouse.heat_total.reserve(layout.size());

    // Populate the array with default values.
    int dim = warehouse.width * warehouse.height;
    for (int i = 0; i < dim; i++)
    {
        warehouse.heat_window.push_back(0);
        warehouse.heat_total.push_back(0);
    }

    return warehouse;
}

/**
 * Move a worker from the start index to the end index. We do not do any error
 * checking since we assuming valid start and end locations.
 */
void Warehouse::move_worker(int start, int end)
{
    // Find the start location.
    auto it = std::find(worker_locs.begin(), worker_locs.end(), start);
    // Remove the start location.
    worker_locs.erase(it);
    // Add the end location.
    worker_locs.push_back(end);

    // Update total heat map.
    heat_total[end] = heat_total[end] + 1;

    // Update statistics.
    curr_moves.push_back(end);
}

/**
 * Marks that all workers have been moved. This is the time to update decaying
 * statistics.
 */
void Warehouse::update_iteration()
{
    // Update the heat window.
    // Our current position in the heat window vector.
    int pos = 0;
    for (auto& sum : heat_window)    
    {
        sum *= DECAY_FACTOR;

        // If we moved to this spot this turn, we will add one to the decaying
        // window for this spot.
        if (std::find(curr_moves.begin(), curr_moves.end(), pos) 
                != curr_moves.end())
        {
            sum += 1;
        }
    
        pos++;
    }

    curr_moves.clear();
}

/**
 * Mark that a contention has occurred at this position. A contention is
 * defined as a 'worker attempting to move to a position which is already
 * occupied by another worker. When marking contentions, mark the spot the 
 * worker attempted to move to. Contentions can be used as one interpretation 
 * of a bottleneck in the warehouse.
 *
 * @param pos The position at which the contention occurred.
 */
void Warehouse::mark_contention(int pos)
{
    // Default value if not found will be 0.
    int& val = contention_spots[pos];
    val++;
}

/**
 * Mark that a deadlock has occurred. A deadlock is defined as a 'worker
 * with no option to move anywhere, and is forced to stay put.' When marking
 * deadlock, mark the position that the worker is stuck in. Deadlocks are one
 * interpretation of a bottleneck in the warehouse.
 *
 * @param pos The position at which the deadlock occurred.
 */
void Warehouse::mark_deadlock(int pos)
{
    // Default value if not found will be 0.
    int& val = deadlock_spots[pos];
    val++;
}


/**
 * Returns the workers in this warehouse.
 *
 * @return A reference to the workers in the warehouse.
 */
std::vector<Worker>& Warehouse::get_workers()
{
    return workers;
}

/**
 * Returns the bins locations in this warehouse.
 *
 * @return The bin locations in the warehouse.
 */
std::vector<int> Warehouse::get_bins()
{
    return bins;
}

/**
 * Returns the layout for the warehouse.
 *
 * @return The layout of the warehouse.
 */
std::vector<int> Warehouse::get_layout()
{
    return layout;
}

/**
 * Returns the height of the warehouse.
 *
 * @return The height of the warehouse.
 */
int Warehouse::get_height()
{
    return height;
}

/**
 * Returns the width of the warehouse.
 *
 * @return The width of the warehouse.
 */
int Warehouse::get_width()
{
    return width;
}

/**
 * Returns the worker locations.
 *
 * @return The locations of the workers in the warehouse.
 */
std::vector<int> Warehouse::get_worker_locs()
{
    return worker_locs;
}

/**
 * Return the wall locations.
 *
 * @return The locations of the walls in the warehouse.
 */
std::vector<int> Warehouse::get_walls()
{
    return walls;
}

/**
 * Returns the drop locations in the warehouse.
 *
 * @return The drop locations in the warehouse.
 */
std::vector<int> Warehouse::get_drops()
{
    return drops;
}

/**
 * Get the total heat map.
 *
 * @return The total heat map.
 */
std::vector<int> Warehouse::get_heat_total()
{
    return heat_total;
}

/**
 * Get the decaying heat map.
 *
 * @param The heat window.
 */
std::vector<double> Warehouse::get_heat_window()
{
    return heat_window;
}

/**
 * Get the deadlock spots.
 *
 * @param The deadlock spots.
 */
std::unordered_map<int,int> Warehouse::get_deadlock_spots()
{
    return deadlock_spots;
}

/**
 * Get the contention spots.
 *
 * @return The contention spots.
 */
std::unordered_map<int,int> Warehouse::get_contention_spots()
{
    return contention_spots;
}
