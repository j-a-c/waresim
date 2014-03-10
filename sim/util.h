#ifndef SIM_UTIL_H
#define SIM_UTIL_H

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
