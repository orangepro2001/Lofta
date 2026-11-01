/* Largest integer scale of the framebuffer that fits a client area, centered.
 * Shared by presentation and tap conversion so the two can never disagree. */
#ifndef LOFTA_VIEWPORT_H
#define LOFTA_VIEWPORT_H

#include "game.h"

typedef struct {
    int scale;
    int x, y; /* top-left of the image in client coordinates */
    int w, h; /* image size in client pixels */
} Viewport;

static inline Viewport viewport_fit(int client_w, int client_h)
{
    Viewport v;
    int sx = client_w / FB_W;
    int sy = client_h / FB_H;
    v.scale = sx < sy ? sx : sy;
    if (v.scale < 1)
        v.scale = 1;
    v.w = FB_W * v.scale;
    v.h = FB_H * v.scale;
    v.x = (client_w - v.w) / 2;
    v.y = (client_h - v.h) / 2;
    return v;
}

/* Converts client coordinates to framebuffer coordinates. Returns 0 when the point
 * is outside the image. */
static inline int viewport_to_fb(const Viewport *v, int cx, int cy, int *fx, int *fy)
{
    int dx = cx - v->x;
    int dy = cy - v->y;
    if (dx < 0 || dy < 0 || dx >= v->w || dy >= v->h)
        return 0;
    *fx = dx / v->scale;
    *fy = dy / v->scale;
    return 1;
}

#endif
