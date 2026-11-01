/* A* over the scene grid, eight directions, no corner cutting. */
#ifndef LOFTA_PATH_H
#define LOFTA_PATH_H

#include <stdint.h>

/* Direction d moves by (PATH_DX[d], PATH_DY[d]). */
extern const int8_t PATH_DX[8];
extern const int8_t PATH_DY[8];

/* Finds a route from (sx, sy) to within stop cells of (tx, ty), writing directions to out.
 * Returns its length, 0 when already there, -1 when there is no route or it exceeds cap. */
int path_find(int sx, int sy, int tx, int ty, int stop, uint8_t *out, int cap);

#endif
