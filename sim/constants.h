#ifndef SIM_CONSTANTS_H
#define SIM_CONSTANTS_H

// Markers used in the Factory layout vector.
const int EMPTY_LOC         = 0;
const int BIN_LOC           = -1;
const int DROP_LOC          = -2;
const int WORKER_LOC        = -3;
const int WALL_LOC          = -4;

// Markers used in the factory layout file.
const char EMPTY_MARKER     = '.';
const char BIN_MARKER       = 'B';
const char DROP_MARKER      = 'D';
const char WORKER_MARKER    = 'W';
const char WALL_MARKER      = 'X';

/**
 * Converts an (x,y) coordinate to a position in an array.
 *
 * @param x x-coordinate.
 * @param y y-coordinate.
 * @param w width.
 */
int coord_to_pos(int x, int y, int w)
{
    return (y * w) + x;
}

#endif
