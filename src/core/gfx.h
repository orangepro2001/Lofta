/* Drawing primitives over the indexed framebuffer. Everything clips to the clip rectangle,
 * so callers never check bounds. */
#ifndef LOFTA_GFX_H
#define LOFTA_GFX_H

#include <stdint.h>

void gfx_target(uint8_t *fb);
void gfx_clip(int x, int y, int w, int h);
void gfx_clip_reset(void);

void gfx_pset(int x, int y, int color);
void gfx_hline(int x, int y, int w, int color);
void gfx_vline(int x, int y, int h, int color);
void gfx_fill(int x, int y, int w, int h, int color);
void gfx_frame(int x, int y, int w, int h, int color);
/* Every other pixel, for shadows and dimmed backdrops. */
void gfx_dither(int x, int y, int w, int h, int color);

/* src is w * h indices, 0 transparent; flip mirrors it left to right. */
void gfx_sprite(const uint8_t *src, int w, int h, int x, int y, int flip);

#endif
