/* 2:1 isometric projection. Cell (x, y) is a square in the world; its top corner lands at
 * world pixel ((x - y) * 16, (x + y) * 8). Positions are in 1/256 of a cell. Integer only. */
#ifndef LOFTA_ISO_H
#define LOFTA_ISO_H

#include <stdint.h>

#define ISO_TW 32
#define ISO_TH 16
#define CELL_FP 256
#define WALL_STEP 8 /* pixels per unit of wall height */

static inline void iso_project(int32_t fx, int32_t fy, int32_t *wx, int32_t *wy)
{
    *wx = (fx - fy) * (ISO_TW / 2) / CELL_FP;
    *wy = (fx + fy) * (ISO_TH / 2) / CELL_FP;
}

/* Inverse of iso_project, for ground level. */
static inline void iso_unproject(int32_t wx, int32_t wy, int32_t *fx, int32_t *fy)
{
    int32_t diff = wx * CELL_FP / (ISO_TW / 2);
    int32_t sum = wy * CELL_FP / (ISO_TH / 2);

    *fx = (sum + diff) / 2;
    *fy = (sum - diff) / 2;
}

#endif
