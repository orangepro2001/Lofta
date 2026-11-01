/* The core's only public header. Hosts and the Lua binding see this and nothing else.
 *
 * The core is deterministic: no clock, no allocation, no floating point, no platform call.
 * The same seed and the same calls give the same state and the same pixels on any compiler. */
#ifndef LOFTA_CORE_API_H
#define LOFTA_CORE_API_H

#include <stddef.h>
#include <stdint.h>

#include "game.h"

#define CORE_RGBA_BYTES (FB_W * FB_H * 4)

/* Returns 0, or the line + 1 of bad sprite art. */
int core_init(uint32_t seed);
void core_tick(void);
uint32_t core_tick_count(void);

/* Frame. */
void core_frame_begin(void);
void core_frame_indexed(const uint8_t **fb, const uint8_t **palette);
void core_frame_rgba(uint8_t *out);

/* Determinism and saves. */
uint64_t core_state_hash(void);
uint64_t core_frame_hash(void);
const void *core_state_bytes(size_t *size);
/* Returns 1 when the blob is a state of this version. Rejected blobs change nothing. */
int core_state_load(const void *data, size_t size);

/* Scene. Kinds are names so scripts never depend on enum order; an unknown name returns -1. */
int core_ground_kind(const char *name);
int core_wall_kind(const char *name);
int core_prop_kind(const char *name);
int core_outfit_kind(const char *name);
/* Clears the world and every actor. Returns 0 when the size does not fit. */
int core_scene_begin(int w, int h);
/* flags: bit 0 block, then windows and doors, see CORE_CELL_*. */
void core_scene_cell(int x, int y, int ground, int height, int wall, int prop, int region, int flags);
void core_scene_cut_region(int region, int cut);

enum {
    CORE_CELL_BLOCK = 1,
    CORE_CELL_WIN_L = 2,
    CORE_CELL_WIN_R = 4,
    CORE_CELL_DOOR_L = 8,
    CORE_CELL_DOOR_R = 16
};

int core_cell_walkable(int x, int y);
int core_cell_region(int x, int y);

/* Actors. Ids are slots; -1 means none. */
int core_actor_spawn(int outfit, int x, int y, int speed);
void core_actor_remove(int id);
void core_actor_cell(int id, int *x, int *y);
int core_actor_moving(int id);
/* Sets a route to within stop cells of (x, y). Returns its length, or -1 without a route. */
int core_actor_walk(int id, int x, int y, int stop);
void core_actor_face(int id, int x, int y);
void core_player_set(int id);

/* Camera and picking, in framebuffer pixels. */
void core_camera_center(int cell_x, int cell_y);
int core_pick_cell(int sx, int sy, int *x, int *y);
int core_pick_actor(int sx, int sy);

/* Day and night: channel multipliers, 256 = full daylight. */
void core_light(int r, int g, int b);

int core_rng(int stream, uint32_t n);

/* Drawing, for scripts. */
void core_draw_world(void);
void core_draw_rect(int x, int y, int w, int h, int color);
void core_draw_dither(int x, int y, int w, int h, int color);
void core_draw_frame(int x, int y, int w, int h, int color);
void core_draw_sprite_person(int x, int y, int outfit);
int core_draw_text(int x, int y, const char *utf8, int color);
int core_draw_text_box(int x, int y, int w, const char *utf8, int color, int max_chars);
int core_text_width(const char *utf8);
int core_text_length(const char *utf8);
/* A palette entry by name ("paper", "ink", ...), or -1. */
int core_color(const char *name);

#endif
