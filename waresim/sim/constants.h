#ifndef SIM_CONSTANTS_H
#define SIM_CONSTANTS_H

/*
 * This file contains some generic constants.
 */

// Markers used in the Warehouse layout vector.
const int EMPTY_LOC         = 0;
const int BIN_LOC           = -1;
const int DROP_LOC          = -2;
const int WORKER_LOC        = -3;
const int WALL_LOC          = -4;

// Markers used in the warehouse layout file.
const char EMPTY_MARKER     = '.';
const char BIN_MARKER       = 'B';
const char DROP_MARKER      = 'D';
const char WORKER_MARKER    = 'W';
const char WALL_MARKER      = 'X';

#endif
