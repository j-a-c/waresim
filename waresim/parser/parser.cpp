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
 *
 * @param to_split The string to split.
 * @param delimiter The delimiter to spit the string by.
 *
 * @return The string resulting from splitting the original string.
 */
std::vector<std::string> Parser::split_string(std::string to_split, char delimiter)
{
    // This will hold separated strings.
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
 *
 * @param filename The file to read.
 *
 * @return A vector containing the lines in the file.
 */
std::vector<std::string> Parser::read_file(std::string filename) 
{
    // Initialize the ifstream.
    std::ifstream ifs(filename);
    if(!ifs.good()) throw("Cannot open file.");

    // This will hold the lines in the file.
    std::vector<std::string> lines;

    for(;;) {
        std::string line;
        std::getline(ifs, line);
        if(!ifs) break;
        lines.push_back(line);
    }
    // Make sure we reached the end of the file.
    if(!ifs.eof()) throw("Error reading file.");

    return lines;
}
