#ifndef SIM_WAREHOUSE_H
#define SIM_WAREHOUSE_H

#include <string>
#include <unordered_map>
#include <vector>

#include "worker.h"

class Warehouse
{
    public:
        Warehouse();
        ~Warehouse();

        // Return the warehouse resulting from parsing the default representation
        // found in the given file.
        static Warehouse parse_default_warehouse(std::string);

        // Move a worker's marker.
        void move_worker(int, int);
        // Mark that a spot has experienced a backoff by some worker at this 
        // position.
        void mark_contention(int);
        // Mark that a spot has experienced a deadlock at this position.
        void mark_deadlock(int);

        // Get the height of the warehouse.
        int get_height();
        // Get the width of the warehouse.
        int get_width();
        // Get the workers in the warehouse.
        std::vector<Worker>& get_workers();
        // Get the worker locations.
        std::vector<int> get_worker_locs();
        // Get the bin locations in the warehouse.
        std::vector<int> get_bins();
        // Get the drop locations in the warehouse.
        std::vector<int> get_drops();
        // Get the wall locations.
        std::vector<int> get_walls();
        // Get the warehouse layout.
        std::vector<int> get_layout();
        // Get the total heat map.
        std::vector<int> get_heat_total();
        // Get the decaying heat map.
        std::vector<double> get_heat_window();
        // Get the deadlock spots.
        std::unordered_map<int,int> get_deadlock_spots();
        // Get the contention spots.
        std::unordered_map<int,int> get_contention_spots();

        // Mark that all workers have been moved.
        void update_iteration();
    private:
        // Reads the file and returns a vector containing its lines.
        static std::vector<std::string> read_file(std::string);

        // The warehouse layout.
        std::vector<int> layout;
        // Bins in the warehouse.
        std::vector<int> bins{};
        // The workers in the warehouse.
        std::vector<Worker> workers{};
        // The locations of the workers in the warehouse.
        std::vector<int> worker_locs{};
        // The walls in the warehouse.
        std::vector<int> walls{};
        // Drop-off locations in the warehouse.
        std::vector<int> drops{};
        // The height and width of the warehouse.
        int height, width;

        /*
         * Statistics
         */
        // An exponentially decaying window of the number of times a spot in
        // the warehouse has been touched. We use float, but this can be changed
        // if precision is important.
        std::vector<double> heat_window{};
        // Holds the total number of times a spot in the warehouse has been
        // touched.
        std::vector<int> heat_total{};
        // Holds the total number of times a deadlock has occurred in a spot.
        std::unordered_map<int,int> deadlock_spots{};
        // Holds the total number of times a conention has occurred in a spot.
        std::unordered_map<int,int> contention_spots{};

        // For exponential decaying window support. Tracks the moves that have
        // been made this iteration.
        std::vector<int> curr_moves{};

        // Decay factor for heat window. Every iteration, each element will be
        // multiplied by this constant. The decay warehouse should be (1-c),
        // where c is a small constant such as 10^-6 or 10^-9 (see Mining of
        // Massive Datasets - RLU (2013)), but we need to tune it depending on
        // how fast we want spots to 'cool off'.
        double DECAY_FACTOR = 1.0 - (0.1);
};

#endif
