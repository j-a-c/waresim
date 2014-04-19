#include "logger.h"

/**
 * Implementation for logger.h
 */

/**
 * Default constructor.
 */
Logger::Logger()
{

}

/**
 * Destructor. Closes the output log file.
 */
Logger::~Logger()
{
    out.close();
}

/**
 * Opens the output log file location for appending.
 *
 * @param loc The location of the log file.
 */
void Logger::set_up(std::string loc)
{
    // Open the output location.
    out.open(loc, 
            std::ios_base::app | std::ios_base::in | std::ios_base::out);
}

/**
 * Logs the message to the output location.
 *
 * @param msg The message to log.
 * @param level The level of the message. The default level is debug.
 */
void Logger::log(std::string msg, LogLevel level)
{ 
    // Formatted time
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char time_buffer [80];
    strftime (time_buffer, 80, "%F %r:\t", timeinfo);

    out << time_buffer << msg << std::endl;
}
