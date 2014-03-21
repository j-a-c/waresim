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
    auto lines = read_file(config);
    for (auto& line : lines)
    {
        auto parts = split_string(line, ':');

        if (parts.size() != 2)
        {
            std::cout << "Invalid format for: " << line << std::endl;
            continue;
        }

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
        else
        {
            std::cout << "Unrecognized var: " << parts[0] << std::endl;
        }
    }
}

int ConfigParser::get_sim_length()
{
    return this->sim_length;
}

std::string ConfigParser::get_warehouse_file()
{
    return this->warehouse;
}

unsigned int ConfigParser::get_seed()
{
    return this->seed;
}
