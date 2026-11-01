/* Everything the core must keep between ticks, in one struct without pointers: saving is
 * copying it, and a hash of it is a hash of the simulation. Private to src/core. */
#ifndef LOFTA_STATE_H
#define LOFTA_STATE_H

#include <stdint.h>

#include "rng.h"

#define SCENE_MAX_W 96
#define SCENE_MAX_H 96
#define ACTOR_MAX 64
#define ACTOR_PATH_MAX 192
#define REGION_MAX 32
#define STATE_MAGIC 0x4C4F4654u
#define STATE_VERSION 1u

enum { GROUND_VOID, GROUND_COBBLE, GROUND_MUD, GROUND_FLAG, GROUND_WOOD, GROUND_WATER, GROUND_KINDS };
enum { WALL_NONE, WALL_BRICK, WALL_PLASTER, WALL_TIMBER, WALL_STONE, WALL_KINDS };
enum { PROP_NONE, PROP_BARREL, PROP_LAMP, PROP_STALL, PROP_TABLE, PROP_CRATE, PROP_KINDS };
enum { CELL_BLOCK = 1, CELL_WIN_L = 2, CELL_WIN_R = 4, CELL_DOOR_L = 8, CELL_DOOR_R = 16 };

typedef struct {
    uint8_t ground, height, wall, prop, region, flags, reserved0, reserved1;
} Cell;

typedef struct {
    uint8_t alive, kind, facing, moving;
    uint8_t stepping, reserved0, reserved1, reserved2;
    int32_t fx, fy;  /* position in 1/256 of a cell; a cell's center is at +128 */
    int32_t goal_x, goal_y; /* center the current step ends on */
    int32_t speed;   /* 1/256 of a cell per tick */
    uint32_t anim;
    uint16_t path_len, path_pos;
    uint8_t path[ACTOR_PATH_MAX];
} Actor;

typedef struct {
    uint32_t magic, version;
    uint32_t tick;
    uint32_t rng[RNG_STREAMS];
    int32_t cam_x, cam_y;
    int32_t scene_w, scene_h;
    int32_t player;
    int32_t tint[3];
    uint8_t region_cut[REGION_MAX];
    Cell cells[SCENE_MAX_H][SCENE_MAX_W];
    Actor actors[ACTOR_MAX];
} CoreState;

/* The one instance. */
extern CoreState g_state;

static inline Cell *state_cell(int x, int y)
{
    return &g_state.cells[y][x];
}

static inline int state_in_scene(int x, int y)
{
    return x >= 0 && y >= 0 && x < g_state.scene_w && y < g_state.scene_h;
}

static inline int state_walkable(int x, int y)
{
    const Cell *c;

    if (!state_in_scene(x, y))
        return 0;
    c = state_cell(x, y);
    return c->ground != GROUND_VOID && c->ground != GROUND_WATER && c->height == 0 &&
           (c->flags & CELL_BLOCK) == 0;
}

#endif
