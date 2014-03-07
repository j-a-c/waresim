#ifndef SIM_FACTORY_H
#define SIM_FACTORY_H

#include <string>
#include <vector>

class Factory
{
    public:
        Factory();
        ~Factory();

        // Return the factory resulting from parsing the default representation
        // found in the given file.
        static Factory parse_default_factory(std::string);
    private:
        // Reads the file and returns a vector containing its lines.
        static std::vector<std::string> read_file(std::string);

        // The factory layout.
        std::vector<int> layout;
        // Number of workers in the factory.
        int num_workers = 0;
        // Bins in the factory.
        std::vector<int> bins{};
        // The location of workers in the factory.
        std::vector<int> worker_locs{};
        // Drop-off locations in the factory.
        std::vector<int> drops{};
        // The height and width of the factory.
        int height, width;

};

#endif
