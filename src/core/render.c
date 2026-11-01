#include "render.h"

#include <stdint.h>

#include "game.h"
#include "gfx.h"
#include "iso.h"
#include "palette.h"
#include "sprites.h"
#include "state.h"

#define CUT_HEIGHT 1 /* wall height left standing when a building is opened up */
#define SORT_MAX ACTOR_MAX

static uint32_t mix(uint32_t a)
{
    a ^= a << 13;
    a ^= a >> 17;
    a ^= a << 5;
    return a;
}

static uint32_t pixel_hash(int x, int y)
{
    return mix((uint32_t)x * 73856093u ^ (uint32_t)y * 19349663u ^ 0x5BD1E995u);
}

/* Ground is patterned in world pixels, so it runs across cell edges without a seam. */
static int ground_color(int kind, int wx, int wy)
{
    int u = wx / 2 + wy;
    int v = wy - wx / 2;

    switch (kind) {
    case GROUND_COBBLE: {
        int su = (u % 7 + 7) % 7;
        int sv = (v % 7 + 7) % 7;
        int cu = (u - su) / 7;
        int cv = (v - sv) / 7;

        if (su == 0 || sv == 0)
            return C_SOOT + 1;
        return C_STONE + 1 + (int)(pixel_hash(cu, cv) % 3u) + (su <= 1 && sv <= 1 ? 1 : 0);
    }
    case GROUND_MUD: {
        uint32_t h = pixel_hash(wx / 3, wy / 2);

        return C_EARTH + 1 + (int)(h % 3u) + ((h >> 8) % 11u == 0u ? 2 : 0);
    }
    case GROUND_FLAG: {
        int su = (u % 24 + 24) % 24;
        int sv = (v % 24 + 24) % 24;

        if (su == 0 || sv == 0)
            return C_STONE + 1;
        return C_STONE + 3 + (int)(pixel_hash((u - su) / 24, (v - sv) / 24) % 2u);
    }
    case GROUND_WOOD: {
        int plank = (v % 12 + 12) % 12;

        if (plank == 0)
            return C_WOOD;
        return C_WOOD + 2 + (int)(pixel_hash((v - plank) / 12, 0) % 2u);
    }
    default:
        return C_BLACK;
    }
}

static int water_color(int wx, int wy, uint32_t tick)
{
    uint32_t h = pixel_hash((wx + (int)(tick / 24u)) / 5, wy / 2);

    return C_WATER + 1 + (int)(h % 3u) + ((h >> 9) % 23u == 0u ? 2 : 0);
}

static void draw_ground(const Cell *c, int wx, int wy, int sx, int sy)
{
    int r;

    for (r = 0; r < ISO_TH; r++) {
        int half = r < ISO_TH / 2 ? (r + 1) * 2 : (ISO_TH - r) * 2;
        int i;

        for (i = -half; i < half; i++) {
            int color = c->ground == GROUND_WATER ? water_color(wx + i, wy + r, g_state.tick)
                                                  : ground_color(c->ground, wx + i, wy + r);

            gfx_pset(sx + i, sy + r, color);
        }
    }
}

static int wall_color(int kind, int column, int row, int face)
{
    int shade = face == 0 ? 0 : 2; /* the light comes from the right */

    switch (kind) {
    case WALL_BRICK: {
        int course = row / 3;
        int offset = (course & 1) ? 3 : 0;

        if (row % 3 == 2 || (column + offset) % 6 == 0)
            return C_SOOT + 2 + shade / 2;
        return C_BRICK + 2 + shade + (int)(pixel_hash((column + offset) / 6, course) % 2u);
    }
    case WALL_PLASTER: {
        int base = C_PLASTER + 1 + shade + (int)(pixel_hash(column / 2, row / 3) % 4u == 0u);

        return row < 2 ? C_WOOD + 1 + shade / 2 : base;
    }
    case WALL_TIMBER: {
        if (column % 8 == 0 || row % 16 == 0)
            return C_WOOD + 1 + shade / 2;
        return C_PLASTER + 1 + shade;
    }
    case WALL_STONE: {
        if (row % 5 == 4 || (column + (row / 5 & 1) * 4) % 8 == 0)
            return C_STONE + 1 + shade / 2;
        return C_STONE + 2 + shade;
    }
    default:
        return C_SOOT + 3;
    }
}

/* A window or door painted on a wall face, following the face's slope. */
static void draw_opening(int sx, int sy, int face, int door, int lit, int lift)
{
    int width = door ? 8 : 6;
    int rise = door ? 22 : 16;
    int n, j;

    for (n = 0; n < width; n++) {
        int i = 4 + n;
        int x = face == 0 ? sx - ISO_TW / 2 + i : sx + i;
        int foot = face == 0 ? sy + ISO_TH / 2 + i / 2 : sy + ISO_TH - 1 - i / 2;

        for (j = lift; j < lift + rise; j++) {
            int color = door ? C_WOOD + 1 + (n == 0 || n == width - 1)
                             : (lit ? C_LAMP + 3 + (j & 1) : C_SOOT + 1);

            if (!door && (n == width / 2 || j == lift + rise / 2))
                color = C_WOOD + 2;
            gfx_pset(x, foot - j, color);
        }
    }
}

static void draw_wall(const Cell *c, int wx, int wy, int sx, int sy, int height)
{
    int pixels = height * WALL_STEP;
    int i, j, r;

    /* Left face, then right. Their shared edge sits under the cell's bottom corner. */
    for (i = 0; i < ISO_TW / 2; i++) {
        int base = sy + ISO_TH / 2 + i / 2;

        for (j = 0; j < pixels; j++)
            gfx_pset(sx - ISO_TW / 2 + i, base - j, wall_color(c->wall, wx - ISO_TW / 2 + i, j, 0));
    }
    for (i = 0; i < ISO_TW / 2; i++) {
        int base = sy + ISO_TH - 1 - i / 2;

        for (j = 0; j < pixels; j++)
            gfx_pset(sx + i, base - j, wall_color(c->wall, wx + i, j, 1));
    }
    if (height > CUT_HEIGHT) {
        int lit = g_state.tint[0] < 200;
        int lift;

        for (lift = 12; lift + 18 < pixels; lift += 26) {
            if (c->flags & CELL_WIN_L)
                draw_opening(sx, sy, 0, 0, lit, lift);
            if (c->flags & CELL_WIN_R)
                draw_opening(sx, sy, 1, 0, lit, lift);
        }
        if (c->flags & CELL_DOOR_L)
            draw_opening(sx, sy, 0, 1, 0, 0);
        if (c->flags & CELL_DOOR_R)
            draw_opening(sx, sy, 1, 1, 0, 0);
    }
    /* Top. A cut wall shows its section; a full one shows slate and a parapet line. */
    for (r = 0; r < ISO_TH; r++) {
        int half = r < ISO_TH / 2 ? (r + 1) * 2 : (ISO_TH - r) * 2;
        int top = sy + r - pixels;

        if (height <= CUT_HEIGHT) {
            gfx_hline(sx - half, top, half * 2, C_SOOT + 2);
        } else {
            int k;

            for (k = -half; k < half; k++) {
                int v = (wy + r) - (wx + k) / 2;
                int band = (v % 6 + 6) % 6;

                gfx_pset(sx + k, top, band == 0 ? C_SOOT + 1 : C_STONE + 1 + (int)(pixel_hash(wx + k, (wy + r) / 3) % 2u));
            }
        }
    }
}

static void draw_shadow(int sx, int sy, int half)
{
    int r;

    for (r = -2; r <= 2; r++) {
        int w = half - (r < 0 ? -r : r) * 2;
        int i;

        for (i = -w; i < w; i += 1)
            if (((sx + i + sy + r) & 1) == 0)
                gfx_pset(sx + i, sy + r, C_SOOT);
    }
}

static void draw_prop(int kind, int sx, int sy)
{
    int cy = sy + ISO_TH / 2;
    int i;

    switch (kind) {
    case PROP_BARREL:
        draw_shadow(sx, cy + 2, 9);
        gfx_fill(sx - 6, cy - 12, 12, 14, C_WOOD + 2);
        gfx_vline(sx - 6, cy - 11, 12, C_WOOD);
        gfx_vline(sx + 5, cy - 11, 12, C_WOOD + 1);
        gfx_vline(sx - 1, cy - 12, 14, C_WOOD + 1);
        gfx_hline(sx - 6, cy - 9, 12, C_SOOT + 3);
        gfx_hline(sx - 6, cy - 2, 12, C_SOOT + 3);
        gfx_fill(sx - 5, cy - 14, 10, 2, C_WOOD + 3);
        break;
    case PROP_LAMP: {
        int lit = g_state.tint[0] < 200;

        draw_shadow(sx, cy + 2, 6);
        gfx_vline(sx, cy - 34, 36, C_SOOT + 2);
        gfx_vline(sx + 1, cy - 34, 36, C_SOOT + 1);
        gfx_fill(sx - 3, cy - 42, 8, 8, lit ? C_LAMP + 4 : C_SOOT + 4);
        gfx_frame(sx - 3, cy - 42, 8, 8, C_SOOT + 1);
        gfx_hline(sx - 4, cy - 43, 10, C_SOOT + 1);
        if (lit) {
            for (i = 0; i < 3; i++)
                gfx_pset(sx + i * 2 - 2, cy - 38, C_LAMP + 5);
        }
        break;
    }
    case PROP_STALL:
        draw_shadow(sx, cy + 3, 13);
        gfx_fill(sx - 11, cy - 8, 22, 10, C_WOOD + 2);
        gfx_hline(sx - 11, cy - 8, 22, C_WOOD + 4);
        gfx_vline(sx - 11, cy - 24, 16, C_WOOD + 1);
        gfx_vline(sx + 10, cy - 24, 16, C_WOOD + 1);
        for (i = 0; i < 5; i++)
            gfx_fill(sx - 13 + i * 6, cy - 28, 3, 5, i & 1 ? C_LINEN + 1 : C_RED + 1);
        gfx_hline(sx - 13, cy - 28, 26, C_WOOD + 1);
        gfx_fill(sx - 7, cy - 13, 3, 5, C_GREEN + 1);
        gfx_fill(sx + 3, cy - 12, 3, 4, C_LINEN + 2);
        break;
    case PROP_TABLE:
        draw_shadow(sx, cy + 2, 10);
        gfx_fill(sx - 9, cy - 8, 18, 3, C_WOOD + 3);
        gfx_vline(sx - 8, cy - 5, 7, C_WOOD + 1);
        gfx_vline(sx + 7, cy - 5, 7, C_WOOD + 1);
        break;
    case PROP_CRATE:
        draw_shadow(sx, cy + 2, 10);
        gfx_fill(sx - 8, cy - 10, 16, 12, C_WOOD + 2);
        gfx_frame(sx - 8, cy - 10, 16, 12, C_WOOD);
        gfx_hline(sx - 8, cy - 5, 16, C_WOOD + 1);
        break;
    default:
        break;
    }
}

static void draw_actor(const Actor *a)
{
    int32_t wx, wy;
    int sx, sy, back, flip, bob;
    const uint8_t *art;

    iso_project(a->fx, a->fy, &wx, &wy);
    sx = wx - g_state.cam_x;
    sy = wy - g_state.cam_y;
    back = a->facing >= 4 && a->facing <= 6;
    /* Directions 4 to 6 walk away from the camera; the left half of the circle mirrors. */
    flip = a->facing == 2 || a->facing == 3 || a->facing == 4;
    bob = a->moving ? (int)((a->anim / 8u) & 1u) : 0;
    art = sprite_person(a->kind, back);
    draw_shadow(sx, sy + 1, 7);
    gfx_sprite(art, PERSON_W, PERSON_H, sx - PERSON_W / 2, sy - PERSON_H + 1 - bob, flip);
}

static int cell_depth(const Actor *a)
{
    return (a->fx >> 8) + (a->fy >> 8);
}

static int actor_before(const Actor *p, const Actor *q)
{
    int32_t dp = p->fx + p->fy;
    int32_t dq = q->fx + q->fy;

    if (dp != dq)
        return dp < dq;
    return p < q;
}

void render_world(void)
{
    const Actor *order[SORT_MAX];
    int count = 0;
    int next = 0;
    int i, x, y, d;

    /* Ground goes down first, so a person between two cells is never cut by the far one. */
    for (y = 0; y < g_state.scene_h; y++) {
        for (x = 0; x < g_state.scene_w; x++) {
            const Cell *c = state_cell(x, y);
            int wx = (x - y) * (ISO_TW / 2);
            int wy = (x + y) * (ISO_TH / 2);
            int sx = wx - g_state.cam_x;
            int sy = wy - g_state.cam_y;

            if (c->ground == GROUND_VOID)
                continue;
            if (sx < -ISO_TW || sx > FB_W + ISO_TW || sy < -ISO_TH || sy > FB_H)
                continue;
            draw_ground(c, wx, wy, sx, sy);
        }
    }

    for (i = 0; i < ACTOR_MAX; i++) {
        const Actor *a = &g_state.actors[i];
        int j;

        if (!a->alive)
            continue;
        j = count++;
        while (j > 0 && actor_before(a, order[j - 1])) {
            order[j] = order[j - 1];
            j--;
        }
        order[j] = a;
    }

    /* Diagonals of constant x + y run from the far corner to the near one. */
    for (d = 0; d <= g_state.scene_w + g_state.scene_h - 2; d++) {
        for (x = 0; x < g_state.scene_w; x++) {
            int32_t wx, wy;
            int sx, sy, height;
            const Cell *c;

            y = d - x;
            if (y < 0 || y >= g_state.scene_h)
                continue;
            c = state_cell(x, y);
            wx = (x - y) * (ISO_TW / 2);
            wy = (x + y) * (ISO_TH / 2);
            sx = wx - g_state.cam_x;
            sy = wy - g_state.cam_y;
            if (sx < -ISO_TW || sx > FB_W + ISO_TW || sy < -ISO_TH)
                continue;
            height = c->height;
            if (height > CUT_HEIGHT && c->region < REGION_MAX && g_state.region_cut[c->region])
                height = CUT_HEIGHT;
            if (sy - height * WALL_STEP > FB_H)
                continue;
            if (height > 0)
                draw_wall(c, wx, wy, sx, sy, height);
            if (c->prop != PROP_NONE)
                draw_prop(c->prop, sx, sy);
        }
        while (next < count && cell_depth(order[next]) <= d)
            draw_actor(order[next++]);
    }
    while (next < count)
        draw_actor(order[next++]);
}
