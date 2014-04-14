#include "util.h"

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

/**
 * Converts a position in an array to an (x,y) coordinate.
 *
 * @param x The x-coordinate to fill.
 * @param y The y-coordinate to fill.
 * @param pos The position in the array.
 * @param w The width.
 */
void pos_to_coord(int *x, int *y, int pos, int w)
{
    *y = pos / w;
    pos -= (*y) * w;
    *x = pos;
}
