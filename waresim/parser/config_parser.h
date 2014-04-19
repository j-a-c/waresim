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
        // Returns the log directory.
        std::string get_log_dir();
        // Returns the view to use.
        std::string get_view();
        // Returns the wait time in between steps.
        int get_wait_time();
        // Returns the dispatch algorithm.
        std::string get_dispatch_algo();
        // Returns the routing algorithm.
        std::string get_routing_algo();
        // Returns the pathfinding algorithm.
        std::string get_path_algo();
    private:
        // The configuration file to parse.
        std::string config{"config"};
        // The seed specified in the config file.
        unsigned int seed = 0;
        // The simulation length specified in the config file.
        int sim_length = 10;
        // The warehouse layout specified in the config file.
        std::string warehouse{"warehouse"};
        // The directory to contain the log files.
        std::string log_dir;
        // The view to use. The default is no view.
        std::string view = "None";
        // The wait time between steps in milliseconds. Default is 1000 ms.
        int wait_time = 1000;
        // The dispatch algorithm. The default is the random dispatch
        // algorithm.
        std::string dispatch = "Rand";
        // The routing algorithm. 
        std::string routing = "Default";
        // The pathfinding algorithm.
        std::string path_algo = "Default";
};

#endif
