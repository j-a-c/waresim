#ifndef WARESIM_WARESIM_LOGGER
#define WARESIM_WARESIM_LOGGER

#include <iostream>
#include <fstream> 
#include <string>
#include <time.h>

enum class LogLevel {Fatal, Error, Warning, Info, Debug, Debug1};

/* Class for logging */
class Logger
{
public:
    Logger();
    ~Logger();
    void set_up(std::string loc);
    void log(std::string msg, LogLevel level = LogLevel::Info);

private: 
    // The log file stream.
    std::ofstream out; 
};

#endif
