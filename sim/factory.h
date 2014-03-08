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

        // Get the workers in the factory.
        std::vector<Worker> get_workers();
    private:
        // Reads the file and returns a vector containing its lines.
        static std::vector<std::string> read_file(std::string);

        // The factory layout.
        std::vector<int> layout;
        // Bins in the factory.
        std::vector<int> bins{};
        // The workers in the factory.
        std::vector<Worker> workers{};
        // Drop-off locations in the factory.
        std::vector<int> drops{};
        // The height and width of the factory.
        int height, width;

};

#endif
