#ifndef PARSER_CONFIG_PARSER_H
#define PARSER_CONFIG_PARSER_H

#include "parser.h"

#include <string>

/**
 * A simple parser to parse the configuration file.
 */
class ConfigParser : public Parser
{
    public:
        ConfigParser(std::string);
        ~ConfigParser();

        // Parse the config file.
        void parse() override;

        // Returns the length of the simulation.
        int get_sim_length();
        // Returns the warehouse file.
        std::string get_warehouse_file();
        // Returns the specified seed.
        unsigned int get_seed();
    private:
        // The configuration file to parse.
        std::string config{"config"};
        // The seed specified in the config file.
        unsigned int seed = 0;
        // The simulation length specified in the config file.
        int sim_length = 10;
        // The warehouse layout specified in the config file.
        std::string warehouse{"warehouse"};
};

#endif
