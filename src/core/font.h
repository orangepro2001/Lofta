/* Glyph table, generated from the 12 px Fusion Pixel font. Sorted by codepoint. */
#ifndef LOFTA_FONT_H
#define LOFTA_FONT_H

#include <stdint.h>

#define FONT_ROWS 12

typedef struct {
    uint16_t codepoint;
    uint8_t advance;
    uint16_t rows[FONT_ROWS]; /* bit 15 is the leftmost pixel */
} Glyph;

extern const Glyph FONT_GLYPHS[];
extern const int FONT_GLYPH_COUNT;

#endif
