#include "gfx.h"

#include <string.h>

#include "game.h"

static uint8_t *g_fb;
static int g_x0, g_y0, g_x1 = FB_W, g_y1 = FB_H;

void gfx_target(uint8_t *fb)
{
    g_fb = fb;
    gfx_clip_reset();
}

void gfx_clip_reset(void)
{
    g_x0 = 0;
    g_y0 = 0;
    g_x1 = FB_W;
    g_y1 = FB_H;
}

void gfx_clip(int x, int y, int w, int h)
{
    g_x0 = x < 0 ? 0 : x;
    g_y0 = y < 0 ? 0 : y;
    g_x1 = x + w > FB_W ? FB_W : x + w;
    g_y1 = y + h > FB_H ? FB_H : y + h;
}

void gfx_pset(int x, int y, int color)
{
    if (g_fb != NULL && x >= g_x0 && x < g_x1 && y >= g_y0 && y < g_y1)
        g_fb[y * FB_W + x] = (uint8_t)color;
}

void gfx_hline(int x, int y, int w, int color)
{
    int x2 = x + w;

    if (g_fb == NULL || y < g_y0 || y >= g_y1)
        return;
    if (x < g_x0)
        x = g_x0;
    if (x2 > g_x1)
        x2 = g_x1;
    if (x2 > x)
        memset(g_fb + y * FB_W + x, color, (size_t)(x2 - x));
}

void gfx_vline(int x, int y, int h, int color)
{
    int i;

    for (i = 0; i < h; i++)
        gfx_pset(x, y + i, color);
}

void gfx_fill(int x, int y, int w, int h, int color)
{
    int i;

    for (i = 0; i < h; i++)
        gfx_hline(x, y + i, w, color);
}

void gfx_frame(int x, int y, int w, int h, int color)
{
    gfx_hline(x, y, w, color);
    gfx_hline(x, y + h - 1, w, color);
    gfx_vline(x, y, h, color);
    gfx_vline(x + w - 1, y, h, color);
}

void gfx_dither(int x, int y, int w, int h, int color)
{
    int i, j;

    for (j = 0; j < h; j++)
        for (i = (x + y + j) & 1; i < w; i += 2)
            gfx_pset(x + i, y + j, color);
}

void gfx_sprite(const uint8_t *src, int w, int h, int x, int y, int flip)
{
    int i, j;

    if (src == NULL)
        return;
    for (j = 0; j < h; j++) {
        for (i = 0; i < w; i++) {
            uint8_t c = src[j * w + (flip ? w - 1 - i : i)];

            if (c != 0)
                gfx_pset(x + i, y + j, c);
        }
    }
}
