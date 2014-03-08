#include <fstream>
#include <string>
#include <vector>

#include "constants.h"
#include "factory.h"
#include "worker.h"

// TODO Delete later, used for debug.
#include <iostream>

/**
 * Constructor.
 */
Factory::Factory()
{
    
}

/**
 * Destructor
 */
Factory::~Factory()
{

}

/**
 * Returns a vector containing the lines in a file.
 *
 * @param file The input file location.
 */
std::vector<std::string> Factory::read_file(std::string filename) 
{
    // Attempts to create stream.
    std::ifstream ifs(filename);
    if(!ifs.good()) throw("Cannot open file.");

    std::vector<std::string> lines;

    // Continue reading lines until we cannot read any more.
    for(;;) {
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
 * Returns the Factory formed from parsing the given file.
 *
 * @param factory_file The file to parse.
 */
Factory Factory::parse_default_factory(std::string factory_file)
{
    Factory factory;

    // Read factory layout file.
    auto lines = read_file(factory_file);

    // Set factory dimensions.
    factory.height = lines.size();
    factory.width = lines.at(0).length();

    // We will use this to hold factory.layout.
    std::vector<int> layout = 
        std::vector<int>(factory.height*factory.width, EMPTY_LOC);
    
    // Populate factory layout.
    for (int y = 0; y < factory.height; y++)
    {
        auto line = lines.at(y);

        for (int x = 0; x < factory.width; x++)
        {
            char marker = line.at(x);

            int pos = coord_to_pos(x, y, factory.width);

            switch (marker)
            {
                case EMPTY_MARKER:
                    // We initialized everything to empty previously.
                    break;
                case BIN_MARKER:
                    layout[pos] = BIN_LOC;
                    factory.bins.push_back(pos);
                    break;
                case DROP_MARKER:
                    layout[pos] = DROP_LOC;
                    factory.drops.push_back(pos);
                    break;
                case WORKER_MARKER:
                    layout[pos] = WORKER_LOC;
                    factory.workers.push_back(Worker(pos));
                    break;
                case WALL_MARKER:
                    layout[pos] = WALL_LOC;
                    break;
                default:
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

    // Set factory layout.
    factory.layout = layout;

    return factory;
}

/**
 * Returns the workers in this factory.
 */
std::vector<Worker> Factory::get_workers()
{
    return workers;
}
