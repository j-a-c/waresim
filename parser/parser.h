#ifndef PARSER_PARSER_H 
#define PARSER_PARSER_H

#include <string>
#include <vector>

/**
 * Represents basic parsing functionality.
 * Include basic functions for reading and splitting lines in a file.
 */
class Parser
{
    public:
        virtual ~Parser();
        virtual void parse() = 0;
    private:
    protected:
        // Splits the line using the char as the delimiter.
        std::vector<std::string> split_string(std::string, char);
        // Returns the lines in the file.
        std::vector<std::string> read_file(std::string);
};

#endif
