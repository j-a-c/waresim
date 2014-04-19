#include "config_parser.h"

#include <iostream>

/**
 * Constructor.
 *
 * @param config The location of the config file to parse.
 */
ConfigParser::ConfigParser(std::string config)
{
    this->config = config;
}

/**
 * Destructor.
 */
ConfigParser::~ConfigParser()
{
 
}

/**
 * Parse the config file.
 */
void ConfigParser::parse()
{
    // Read teh lines in the file.
    auto lines = read_file(config);
    // Parse each line.
    for (auto& line : lines)
    {
        // Split the line by the delimiter.
        auto parts = split_string(line, ':');

        // If there are more parts that we expected, we will skip this continue
        // and continue.
        if (parts.size() != 2)
        {
            std::cout << "Invalid format for: " << line << std::endl;
            continue;
        }

        // Rename the parts.
        std::string key = parts[0];
        std::string value = parts[1];

        if (key == "Length")
        {
            sim_length = std::stoi(value);
        }
        else if (key == "Warehouse")
        {
            warehouse = value;
        }
        else if (key == "Seed")
        {
            // We use stol because seed is an unsigned int, so we need the
            // extra space.
            seed = std::stol(value);
        }
        else if (key == "Log")
        {
            log_dir = value;
        }
        else if (key == "View")
        {
            view = value;
        }
        else if (key == "Wait")
        {
            wait_time = std::stoi(value);
        }
        else // Print a simple error if we do not recognize this parameter.
        {
            std::cout << "Unrecognized var: " << parts[0] << std::endl;
        }
    }
}

/**
 * Returns the simulation length parsed from the configuration file.
 */
int ConfigParser::get_sim_length()
{
    return this->sim_length;
}

/**
 * Returns the warehouse file location parsed from the configuration file.
 */
std::string ConfigParser::get_warehouse_file()
{
    return this->warehouse;
}

/**
 * Returns the seed parsed from the configuration file.
 */
unsigned int ConfigParser::get_seed()
{
    return this->seed;
}

/**
 * Returns the log directory.
 */
std::string ConfigParser::get_log_dir()
{
    return log_dir;
}

/**
 * Returns the view to use.
 */
std::string ConfigParser::get_view()
{
    return view;
}

int ConfigParser::get_wait_time()
{
    return wait_time;
}
