#ifndef SIM_FACTORY_H
#define SIM_FACTORY_H

#include <string>
#include <vector>

#include "worker.h"

class Factory
{
    public:
        Factory();
        ~Factory();

        // Return the factory resulting from parsing the default representation
        // found in the given file.
        static Factory parse_default_factory(std::string);

        // Move a worker's marker.
        void move_worker(int, int);

        // Get the height of the factory.
        int get_height();
        // Get the width of the factory.
        int get_width();
        // Get the workers in the factory.
        std::vector<Worker>& get_workers();
        // Get the worker locations.
        std::vector<int> get_worker_locs();
        // Get the bin locations in the factory.
        std::vector<int> get_bins();
        // Get the drop locations in the factory.
        std::vector<int> get_drops();
        // Get the wall locations.
        std::vector<int> get_walls();
        // Get the factory layout.
        std::vector<int> get_layout();
    private:
        // Reads the file and returns a vector containing its lines.
        static std::vector<std::string> read_file(std::string);

        // The factory layout.
        std::vector<int> layout;
        // Bins in the factory.
        std::vector<int> bins{};
        // The workers in the factory.
        std::vector<Worker> workers{};
        // The locations of the workers in the factory.
        std::vector<int> worker_locs{};
        // The walls in the factory.
        std::vector<int> walls{};
        // Drop-off locations in the factory.
        std::vector<int> drops{};
        // The height and width of the factory.
        int height, width;

};

#endif
