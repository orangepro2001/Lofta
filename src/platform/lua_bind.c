#include "lua_bind.h"

#include <lauxlib.h>
#include <stdio.h>

#include "core_api.h"

static int kind_arg(lua_State *L, int index, int (*fn)(const char *))
{
    const char *name = luaL_checkstring(L, index);
    int kind = fn(name);

    if (kind < 0)
        return luaL_error(L, "unknown name '%s'", name);
    return kind;
}

static int int_arg(lua_State *L, int index)
{
    return (int)luaL_checkinteger(L, index);
}

static int l_init(lua_State *L)
{
    int bad = core_init((uint32_t)luaL_checkinteger(L, 1));

    if (bad != 0)
        return luaL_error(L, "bad sprite art at line %d", bad - 1);
    return 0;
}

static int l_tick(lua_State *L)
{
    (void)L;
    core_tick();
    return 0;
}

static int l_tick_count(lua_State *L)
{
    lua_pushinteger(L, (lua_Integer)core_tick_count());
    return 1;
}

static void push_hex(lua_State *L, uint64_t value)
{
    char text[20];

    snprintf(text, sizeof text, "%08x%08x", (unsigned int)(value >> 32), (unsigned int)(value & 0xFFFFFFFFu));
    lua_pushstring(L, text);
}

static int l_state_hash(lua_State *L)
{
    push_hex(L, core_state_hash());
    return 1;
}

static int l_frame_hash(lua_State *L)
{
    push_hex(L, core_frame_hash());
    return 1;
}

static int l_state_blob(lua_State *L)
{
    size_t size = 0;
    const void *data = core_state_bytes(&size);

    lua_pushlstring(L, (const char *)data, size);
    return 1;
}

static int l_state_load(lua_State *L)
{
    size_t size = 0;
    const char *data = luaL_checklstring(L, 1, &size);

    lua_pushboolean(L, core_state_load(data, size));
    return 1;
}

static int l_scene_begin(lua_State *L)
{
    lua_pushboolean(L, core_scene_begin(int_arg(L, 1), int_arg(L, 2)));
    return 1;
}

/* scene_cell(x, y, ground, height, wall, prop, region, flags), names for the three kinds. */
static int l_scene_cell(lua_State *L)
{
    core_scene_cell(int_arg(L, 1), int_arg(L, 2), kind_arg(L, 3, core_ground_kind), int_arg(L, 4),
                    kind_arg(L, 5, core_wall_kind), kind_arg(L, 6, core_prop_kind), int_arg(L, 7),
                    int_arg(L, 8));
    return 0;
}

static int l_scene_cut_region(lua_State *L)
{
    core_scene_cut_region(int_arg(L, 1), lua_toboolean(L, 2));
    return 0;
}

static int l_cell_walkable(lua_State *L)
{
    lua_pushboolean(L, core_cell_walkable(int_arg(L, 1), int_arg(L, 2)));
    return 1;
}

static int l_cell_region(lua_State *L)
{
    lua_pushinteger(L, core_cell_region(int_arg(L, 1), int_arg(L, 2)));
    return 1;
}

static int l_actor_spawn(lua_State *L)
{
    int id = core_actor_spawn(kind_arg(L, 1, core_outfit_kind), int_arg(L, 2), int_arg(L, 3),
                              (int)luaL_optinteger(L, 4, 6));

    if (id < 0)
        return luaL_error(L, "cannot spawn an actor here");
    lua_pushinteger(L, id);
    return 1;
}

static int l_actor_remove(lua_State *L)
{
    core_actor_remove(int_arg(L, 1));
    return 0;
}

static int l_actor_cell(lua_State *L)
{
    int x, y;

    core_actor_cell(int_arg(L, 1), &x, &y);
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    return 2;
}

static int l_actor_moving(lua_State *L)
{
    lua_pushboolean(L, core_actor_moving(int_arg(L, 1)));
    return 1;
}

static int l_actor_walk(lua_State *L)
{
    lua_pushinteger(L, core_actor_walk(int_arg(L, 1), int_arg(L, 2), int_arg(L, 3),
                                       (int)luaL_optinteger(L, 4, 0)));
    return 1;
}

static int l_actor_face(lua_State *L)
{
    core_actor_face(int_arg(L, 1), int_arg(L, 2), int_arg(L, 3));
    return 0;
}

static int l_player_set(lua_State *L)
{
    core_player_set(int_arg(L, 1));
    return 0;
}

static int l_camera_center(lua_State *L)
{
    core_camera_center(int_arg(L, 1), int_arg(L, 2));
    return 0;
}

static int l_pick_cell(lua_State *L)
{
    int x, y;

    if (!core_pick_cell(int_arg(L, 1), int_arg(L, 2), &x, &y)) {
        lua_pushnil(L);
        return 1;
    }
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);
    return 2;
}

static int l_pick_actor(lua_State *L)
{
    int id = core_pick_actor(int_arg(L, 1), int_arg(L, 2));

    if (id < 0)
        lua_pushnil(L);
    else
        lua_pushinteger(L, id);
    return 1;
}

static int l_light(lua_State *L)
{
    core_light(int_arg(L, 1), int_arg(L, 2), int_arg(L, 3));
    return 0;
}

static int l_rng(lua_State *L)
{
    static const char *const STREAMS[] = {"world", "story", "fx", NULL};

    lua_pushinteger(L, core_rng(luaL_checkoption(L, 1, NULL, STREAMS), (uint32_t)luaL_checkinteger(L, 2)));
    return 1;
}

static int color_arg(lua_State *L, int index)
{
    return kind_arg(L, index, core_color);
}

static int l_draw_world(lua_State *L)
{
    (void)L;
    core_draw_world();
    return 0;
}

static int l_draw_rect(lua_State *L)
{
    core_draw_rect(int_arg(L, 1), int_arg(L, 2), int_arg(L, 3), int_arg(L, 4), color_arg(L, 5));
    return 0;
}

static int l_draw_dither(lua_State *L)
{
    core_draw_dither(int_arg(L, 1), int_arg(L, 2), int_arg(L, 3), int_arg(L, 4), color_arg(L, 5));
    return 0;
}

static int l_draw_frame(lua_State *L)
{
    core_draw_frame(int_arg(L, 1), int_arg(L, 2), int_arg(L, 3), int_arg(L, 4), color_arg(L, 5));
    return 0;
}

static int l_draw_person(lua_State *L)
{
    core_draw_sprite_person(int_arg(L, 1), int_arg(L, 2), kind_arg(L, 3, core_outfit_kind));
    return 0;
}

static int l_draw_text(lua_State *L)
{
    lua_pushinteger(L, core_draw_text(int_arg(L, 1), int_arg(L, 2), luaL_checkstring(L, 3), color_arg(L, 4)));
    return 1;
}

/* text_box(x, y, w, text, color, max_chars): color nil measures without drawing. */
static int l_draw_text_box(lua_State *L)
{
    int color = lua_isnil(L, 5) ? -1 : color_arg(L, 5);

    lua_pushinteger(L, core_draw_text_box(int_arg(L, 1), int_arg(L, 2), int_arg(L, 3), luaL_checkstring(L, 4),
                                          color, (int)luaL_optinteger(L, 6, -1)));
    return 1;
}

static int l_text_width(lua_State *L)
{
    lua_pushinteger(L, core_text_width(luaL_checkstring(L, 1)));
    return 1;
}

static int l_text_length(lua_State *L)
{
    lua_pushinteger(L, core_text_length(luaL_checkstring(L, 1)));
    return 1;
}

static const luaL_Reg CORE_FUNCS[] = {
    {"init", l_init},
    {"tick", l_tick},
    {"tick_count", l_tick_count},
    {"state_hash", l_state_hash},
    {"frame_hash", l_frame_hash},
    {"state_blob", l_state_blob},
    {"state_load", l_state_load},
    {"scene_begin", l_scene_begin},
    {"scene_cell", l_scene_cell},
    {"scene_cut_region", l_scene_cut_region},
    {"cell_walkable", l_cell_walkable},
    {"cell_region", l_cell_region},
    {"actor_spawn", l_actor_spawn},
    {"actor_remove", l_actor_remove},
    {"actor_cell", l_actor_cell},
    {"actor_moving", l_actor_moving},
    {"actor_walk", l_actor_walk},
    {"actor_face", l_actor_face},
    {"player_set", l_player_set},
    {"camera_center", l_camera_center},
    {"pick_cell", l_pick_cell},
    {"pick_actor", l_pick_actor},
    {"light", l_light},
    {"rng", l_rng},
    {"draw_world", l_draw_world},
    {"draw_rect", l_draw_rect},
    {"draw_dither", l_draw_dither},
    {"draw_frame", l_draw_frame},
    {"draw_person", l_draw_person},
    {"draw_text", l_draw_text},
    {"text_box", l_draw_text_box},
    {"text_width", l_text_width},
    {"text_length", l_text_length},
    {NULL, NULL},
};

void lua_bind_core(lua_State *L)
{
    luaL_newlib(L, CORE_FUNCS);
    lua_pushinteger(L, FB_W);
    lua_setfield(L, -2, "fb_w");
    lua_pushinteger(L, FB_H);
    lua_setfield(L, -2, "fb_h");
    lua_setglobal(L, "core");
}
