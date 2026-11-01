#include "core_api.h"

#include <string.h>

#include "gfx.h"
#include "iso.h"
#include "palette.h"
#include "path.h"
#include "render.h"
#include "sprites.h"
#include "state.h"
#include "text.h"

#define CAMERA_EASE 8
#define PICK_HALF_W 8
#define FNV_OFFSET 0xCBF29CE484222325ull
#define FNV_PRIME 0x100000001B3ull
#define DIAGONAL_NUM 181 /* 256 / sqrt 2 */

CoreState g_state;

static uint8_t g_fb[FB_W * FB_H];
static uint8_t g_palette[256][3];

typedef struct {
    const char *name;
    int value;
} NamedValue;

static const NamedValue GROUNDS[] = {
    {"void", GROUND_VOID}, {"cobble", GROUND_COBBLE}, {"mud", GROUND_MUD},
    {"flag", GROUND_FLAG}, {"wood", GROUND_WOOD},     {"water", GROUND_WATER},
};
static const NamedValue WALLS[] = {
    {"none", WALL_NONE},       {"brick", WALL_BRICK}, {"plaster", WALL_PLASTER},
    {"timber", WALL_TIMBER},   {"stone", WALL_STONE},
};
static const NamedValue PROPS[] = {
    {"none", PROP_NONE},   {"barrel", PROP_BARREL}, {"lamp", PROP_LAMP},
    {"stall", PROP_STALL}, {"table", PROP_TABLE},   {"crate", PROP_CRATE},
};
static const NamedValue OUTFIT_NAMES[] = {
    {"searcher", OUTFIT_SEARCHER},   {"matron", OUTFIT_MATRON},     {"constable", OUTFIT_CONSTABLE},
    {"gentleman", OUTFIT_GENTLEMAN}, {"labourer", OUTFIT_LABOURER},
};
static const NamedValue COLORS[] = {
    {"black", C_BLACK},      {"ink", C_INK},         {"paper", C_PAPER},       {"paper_dim", C_PAPER_DIM},
    {"white", C_WHITE},      {"panel", C_UI_PANEL},  {"edge", C_UI_EDGE},      {"hilite", C_UI_HILITE},
    {"warn", C_UI_WARN},     {"good", C_UI_GOOD},    {"muted", C_UI_MUTED},    {"rule", C_UI_RULE},
    {"shade", C_UI_SHADE},
};

static int lookup(const NamedValue *table, size_t count, const char *name)
{
    size_t i;

    for (i = 0; i < count; i++)
        if (strcmp(table[i].name, name) == 0)
            return table[i].value;
    return -1;
}

#define LOOKUP(table, name) lookup(table, sizeof table / sizeof table[0], name)

int core_ground_kind(const char *name) { return LOOKUP(GROUNDS, name); }
int core_wall_kind(const char *name) { return LOOKUP(WALLS, name); }
int core_prop_kind(const char *name) { return LOOKUP(PROPS, name); }
int core_outfit_kind(const char *name) { return LOOKUP(OUTFIT_NAMES, name); }
int core_color(const char *name) { return LOOKUP(COLORS, name); }

int core_init(uint32_t seed)
{
    int bad;

    memset(&g_state, 0, sizeof g_state);
    g_state.magic = STATE_MAGIC;
    g_state.version = STATE_VERSION;
    g_state.player = -1;
    g_state.tint[0] = g_state.tint[1] = g_state.tint[2] = 256;
    rng_seed(g_state.rng, seed);
    gfx_target(g_fb);
    bad = sprites_bake();
    return bad;
}

static int valid_actor(int id)
{
    return id >= 0 && id < ACTOR_MAX && g_state.actors[id].alive;
}

static void cell_center(int x, int y, int32_t *fx, int32_t *fy)
{
    *fx = x * CELL_FP + CELL_FP / 2;
    *fy = y * CELL_FP + CELL_FP / 2;
}

static int32_t step_toward(int32_t from, int32_t to, int32_t amount)
{
    if (from < to)
        return from + amount > to ? to : from + amount;
    return from - amount < to ? to : from - amount;
}

/* Steps run center to center, so a route is always a chain of whole cells. */
static void advance_actor(Actor *a)
{
    int d;
    int32_t v;

    if (!a->stepping) {
        if (a->path_pos >= a->path_len) {
            a->moving = 0;
            return;
        }
        /* Taken off the route as it starts, so a new route can never lose its first step. */
        d = a->path[a->path_pos++];
        a->facing = (uint8_t)d;
        a->goal_x = a->fx + PATH_DX[d] * CELL_FP;
        a->goal_y = a->fy + PATH_DY[d] * CELL_FP;
        a->stepping = 1;
        a->moving = 1;
    }
    a->anim++;
    d = a->facing;
    v = (d & 1) ? a->speed * DIAGONAL_NUM / CELL_FP : a->speed;
    if (v < 1)
        v = 1;
    a->fx = step_toward(a->fx, a->goal_x, v);
    a->fy = step_toward(a->fy, a->goal_y, v);
    if (a->fx == a->goal_x && a->fy == a->goal_y) {
        a->stepping = 0;
        a->moving = a->path_pos < a->path_len;
    }
}

static void update_camera(void)
{
    const Actor *a;
    int32_t wx, wy, tx, ty;

    if (!valid_actor(g_state.player))
        return;
    a = &g_state.actors[g_state.player];
    iso_project(a->fx, a->fy, &wx, &wy);
    tx = wx - FB_W / 2;
    ty = wy - FB_H / 2 - PICK_HALF_W;
    g_state.cam_x += (tx - g_state.cam_x) / CAMERA_EASE;
    g_state.cam_y += (ty - g_state.cam_y) / CAMERA_EASE;
}

void core_tick(void)
{
    int i;

    g_state.tick++;
    for (i = 0; i < ACTOR_MAX; i++)
        if (g_state.actors[i].alive)
            advance_actor(&g_state.actors[i]);
    update_camera();
}

uint32_t core_tick_count(void)
{
    return g_state.tick;
}

void core_frame_begin(void)
{
    gfx_target(g_fb);
    memset(g_fb, C_BLACK, sizeof g_fb);
    palette_build(g_palette, g_state.tint);
}

void core_frame_indexed(const uint8_t **fb, const uint8_t **palette)
{
    *fb = g_fb;
    *palette = &g_palette[0][0];
}

void core_frame_rgba(uint8_t *out)
{
    size_t i;

    for (i = 0; i < sizeof g_fb; i++) {
        const uint8_t *c = g_palette[g_fb[i]];

        out[i * 4 + 0] = c[0];
        out[i * 4 + 1] = c[1];
        out[i * 4 + 2] = c[2];
        out[i * 4 + 3] = 255;
    }
}

static uint64_t fnv1a(const void *data, size_t size, uint64_t hash)
{
    const uint8_t *p = (const uint8_t *)data;
    size_t i;

    for (i = 0; i < size; i++) {
        hash ^= p[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

uint64_t core_state_hash(void)
{
    return fnv1a(&g_state, sizeof g_state, FNV_OFFSET);
}

uint64_t core_frame_hash(void)
{
    return fnv1a(g_palette, sizeof g_palette, fnv1a(g_fb, sizeof g_fb, FNV_OFFSET));
}

const void *core_state_bytes(size_t *size)
{
    *size = sizeof g_state;
    return &g_state;
}

int core_state_load(const void *data, size_t size)
{
    uint32_t head[2];

    if (size != sizeof g_state)
        return 0;
    memcpy(head, data, sizeof head);
    if (head[0] != STATE_MAGIC || head[1] != STATE_VERSION)
        return 0;
    memcpy(&g_state, data, size);
    return 1;
}

int core_scene_begin(int w, int h)
{
    if (w <= 0 || h <= 0 || w > SCENE_MAX_W || h > SCENE_MAX_H)
        return 0;
    memset(g_state.cells, 0, sizeof g_state.cells);
    memset(g_state.actors, 0, sizeof g_state.actors);
    memset(g_state.region_cut, 0, sizeof g_state.region_cut);
    g_state.scene_w = w;
    g_state.scene_h = h;
    g_state.player = -1;
    return 1;
}

void core_scene_cell(int x, int y, int ground, int height, int wall, int prop, int region, int flags)
{
    Cell *c;

    if (!state_in_scene(x, y))
        return;
    c = state_cell(x, y);
    c->ground = (uint8_t)ground;
    c->height = (uint8_t)height;
    c->wall = (uint8_t)wall;
    c->prop = (uint8_t)prop;
    c->region = (uint8_t)region;
    c->flags = (uint8_t)flags;
}

void core_scene_cut_region(int region, int cut)
{
    if (region >= 0 && region < REGION_MAX)
        g_state.region_cut[region] = cut ? 1 : 0;
}

int core_cell_walkable(int x, int y)
{
    return state_walkable(x, y);
}

int core_cell_region(int x, int y)
{
    return state_in_scene(x, y) ? state_cell(x, y)->region : 0;
}

int core_actor_spawn(int outfit, int x, int y, int speed)
{
    int i;

    if (outfit < 0 || outfit >= OUTFIT_KINDS || !state_in_scene(x, y))
        return -1;
    for (i = 0; i < ACTOR_MAX; i++) {
        Actor *a = &g_state.actors[i];

        if (a->alive)
            continue;
        memset(a, 0, sizeof *a);
        a->alive = 1;
        a->kind = (uint8_t)outfit;
        a->facing = 1;
        a->speed = speed;
        cell_center(x, y, &a->fx, &a->fy);
        return i;
    }
    return -1;
}

void core_actor_remove(int id)
{
    if (valid_actor(id))
        g_state.actors[id].alive = 0;
}

void core_actor_cell(int id, int *x, int *y)
{
    *x = -1;
    *y = -1;
    if (valid_actor(id)) {
        *x = g_state.actors[id].fx >> 8;
        *y = g_state.actors[id].fy >> 8;
    }
}

int core_actor_moving(int id)
{
    return valid_actor(id) && g_state.actors[id].moving;
}

int core_actor_walk(int id, int x, int y, int stop)
{
    Actor *a;
    uint8_t route[ACTOR_PATH_MAX];
    int cx, cy, length;

    if (!valid_actor(id))
        return -1;
    a = &g_state.actors[id];
    /* A walker mid-step finishes the step, so a new route starts from where it will land. */
    cx = (a->stepping ? a->goal_x : a->fx) >> 8;
    cy = (a->stepping ? a->goal_y : a->fy) >> 8;
    length = path_find(cx, cy, x, y, stop, route, ACTOR_PATH_MAX);
    if (length < 0)
        return -1;
    memcpy(a->path, route, (size_t)length);
    a->path_len = (uint16_t)length;
    a->path_pos = 0;
    a->moving = a->stepping || length > 0;
    return length;
}

void core_actor_face(int id, int x, int y)
{
    Actor *a;
    int dx, dy, d;

    if (!valid_actor(id))
        return;
    a = &g_state.actors[id];
    dx = x - (a->fx >> 8);
    dy = y - (a->fy >> 8);
    dx = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    dy = dy > 0 ? 1 : (dy < 0 ? -1 : 0);
    for (d = 0; d < 8; d++)
        if (PATH_DX[d] == dx && PATH_DY[d] == dy)
            a->facing = (uint8_t)d;
}

void core_player_set(int id)
{
    g_state.player = valid_actor(id) ? id : -1;
    if (g_state.player >= 0) {
        const Actor *a = &g_state.actors[id];
        int32_t wx, wy;

        iso_project(a->fx, a->fy, &wx, &wy);
        g_state.cam_x = wx - FB_W / 2;
        g_state.cam_y = wy - FB_H / 2 - PICK_HALF_W;
    }
}

void core_camera_center(int cell_x, int cell_y)
{
    int32_t fx, fy, wx, wy;

    cell_center(cell_x, cell_y, &fx, &fy);
    iso_project(fx, fy, &wx, &wy);
    g_state.cam_x = wx - FB_W / 2;
    g_state.cam_y = wy - FB_H / 2;
}

int core_pick_cell(int sx, int sy, int *x, int *y)
{
    int32_t fx, fy;

    iso_unproject(sx + g_state.cam_x, sy + g_state.cam_y, &fx, &fy);
    /* Shift before the division: a negative position must not round toward cell zero. */
    *x = fx >> 8;
    *y = fy >> 8;
    return state_in_scene(*x, *y);
}

int core_pick_actor(int sx, int sy)
{
    int best = -1;
    int32_t best_depth = -2147483647;
    int i;

    for (i = 0; i < ACTOR_MAX; i++) {
        const Actor *a = &g_state.actors[i];
        int32_t wx, wy, ax, ay;

        if (!a->alive)
            continue;
        iso_project(a->fx, a->fy, &wx, &wy);
        ax = wx - g_state.cam_x;
        ay = wy - g_state.cam_y;
        if (sx < ax - PICK_HALF_W || sx >= ax + PICK_HALF_W || sy < ay - 30 || sy > ay + 2)
            continue;
        if (a->fx + a->fy >= best_depth) {
            best = i;
            best_depth = a->fx + a->fy;
        }
    }
    return best;
}

void core_light(int r, int g, int b)
{
    g_state.tint[0] = r;
    g_state.tint[1] = g;
    g_state.tint[2] = b;
}

int core_rng(int stream, uint32_t n)
{
    if (stream < 0 || stream >= RNG_STREAMS || n == 0u)
        return 0;
    return (int)rng_range(g_state.rng, stream, n);
}

void core_draw_world(void) { render_world(); }
void core_draw_rect(int x, int y, int w, int h, int color) { gfx_fill(x, y, w, h, color); }
void core_draw_dither(int x, int y, int w, int h, int color) { gfx_dither(x, y, w, h, color); }
void core_draw_frame(int x, int y, int w, int h, int color) { gfx_frame(x, y, w, h, color); }

void core_draw_sprite_person(int x, int y, int outfit)
{
    gfx_sprite(sprite_person(outfit, 0), PERSON_W, PERSON_H, x, y, 0);
}

int core_draw_text(int x, int y, const char *utf8, int color) { return text_draw(x, y, utf8, color); }

int core_draw_text_box(int x, int y, int w, const char *utf8, int color, int max_chars)
{
    return text_box(x, y, w, utf8, color, max_chars);
}

int core_text_width(const char *utf8) { return text_width(utf8); }
int core_text_length(const char *utf8) { return text_length(utf8); }
