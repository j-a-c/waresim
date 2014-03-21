#include "parser.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/**
 * Destructor.
 */
Parser::~Parser()
{

}

/*
 * Some parser-related functions, including reading a file and splitting a
 * string using a specified delimiter.
 */

/*
 * Splits the string using the specified delimiter. 
 */
std::vector<std::string> Parser::split_string(std::string to_split, char delimiter)
{
    // The separated strings.
    std::vector<std::string> strings;

    std::istringstream f{to_split};
    std::string s;    
    while (std::getline(f, s, delimiter)) 
    {
        strings.push_back(s);
    }

    return strings;
}

/*
 * Returns a vector where each element is a line of the file.
 * The order of the lines in the file is preserved.
 *
 * Throws a parse_exception is the file cannot be opened or it there was an
 * error reading the file.
 */
std::vector<std::string> Parser::read_file(std::string filename) 
{
    std::ifstream ifs(filename);
    if(!ifs.good()) throw("Cannot open file.");

    std::vector<std::string> lines;

    for(;;) {
        std::string line;
        std::getline(ifs, line);
        if(!ifs) break;
        lines.push_back(line);
    }
    if(!ifs.eof()) throw("Error reading file.");

    return lines;
}
