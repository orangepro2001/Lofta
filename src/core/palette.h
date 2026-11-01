/* Indexed palette. Entries below PAL_TINT_FROM are interface colors that never change; the
 * rest are world colors that the time of day recolors each frame without touching pixels. */
#ifndef LOFTA_PALETTE_H
#define LOFTA_PALETTE_H

#include <stdint.h>

enum {
    C_CLEAR = 0, /* sprite transparency key; black when it reaches the screen */
    C_BLACK,
    C_INK,
    C_PAPER,
    C_PAPER_DIM,
    C_WHITE,
    C_UI_PANEL,
    C_UI_EDGE,
    C_UI_HILITE,
    C_UI_WARN,
    C_UI_GOOD,
    C_UI_MUTED,
    C_UI_RULE,
    C_UI_SHADE,
    PAL_TINT_FROM = 16,
    C_SOOT = 16,    /* 8 steps, dark to light, as are the ramps below */
    C_BRICK = 24,   /* 8 */
    C_EARTH = 32,   /* 8 */
    C_STONE = 40,   /* 8 */
    C_WOOD = 48,    /* 6 */
    C_PLASTER = 54, /* 6 */
    C_WATER = 60,   /* 6 */
    C_SKIN = 66,    /* 5 */
    C_SKIN_DK = 71, /* 3 */
    C_RED = 74,     /* 4 */
    C_BLUE = 78,    /* 4 */
    C_GREEN = 82,   /* 4 */
    C_LINEN = 86,   /* 3 */
    PAL_TINT_END = 89,
    C_LAMP = 89, /* 6, keeps its light after dark */
    PAL_COLORS = 95
};

void palette_build(uint8_t out[256][3], const int32_t tint[3]);

#endif
