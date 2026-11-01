#include "lua_host.h"

#include <lauxlib.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua_bind.h"
#include "savefile.h"
#include "version.h"

#define LOADED_KEY "lofta.loaded"
#define MAX_PATH_CHARS 1024

static LuaHostConfig g_config;

static void report(const char *context, const char *message)
{
    if (g_config.on_error != NULL)
        g_config.on_error(context, message);
}

static int traceback(lua_State *L)
{
    const char *msg = lua_tostring(L, 1);
    luaL_traceback(L, L, msg != NULL ? msg : "(error object is not a string)", 1);
    return 1;
}

int lua_host_pcall(lua_State *L, int nargs, int nresults, const char *context)
{
    int base = lua_gettop(L) - nargs;
    int status;

    lua_pushcfunction(L, traceback);
    lua_insert(L, base);
    status = lua_pcall(L, nargs, nresults, base);
    lua_remove(L, base);
    if (status != LUA_OK) {
        const char *msg = lua_tostring(L, -1);
        report(context, msg != NULL ? msg : "(no message)");
        lua_pop(L, 1);
        return 0;
    }
    return 1;
}

/* Tries the search folders, then the scripts built into the program. Leaves the chunk on
 * the stack and returns 1, or returns 0 with an error message on the stack. */
static int load_chunk(lua_State *L, const char *module)
{
    char relative[MAX_PATH_CHARS];
    size_t i;
    const EmbeddedScript *s;

    snprintf(relative, sizeof relative, "%s", module);
    for (i = 0; relative[i] != '\0'; i++)
        if (relative[i] == '.')
            relative[i] = '/';

    for (i = 0; i < sizeof g_config.dirs / sizeof g_config.dirs[0]; i++) {
        char path[MAX_PATH_CHARS * 2];
        FILE *f;

        if (g_config.dirs[i] == NULL)
            continue;
        snprintf(path, sizeof path, "%s/%s.lua", g_config.dirs[i], relative);
        f = fopen(path, "rb");
        if (f == NULL)
            continue;
        fclose(f);
        return luaL_loadfilex(L, path, "t") == LUA_OK;
    }

    for (s = EMBEDDED_SCRIPTS; s->name != NULL; s++)
        if (strcmp(s->name, module) == 0)
            return luaL_loadbufferx(L, s->data, s->size, s->name, "t") == LUA_OK;

    lua_pushfstring(L, "module '%s' not found", module);
    return 0;
}

/* The only way to load code. It knows the search folders and the built-in scripts, and
 * nothing else on the machine. */
static int l_require(lua_State *L)
{
    const char *module = luaL_checkstring(L, 1);

    lua_getfield(L, LUA_REGISTRYINDEX, LOADED_KEY);
    lua_getfield(L, -1, module);
    if (!lua_isnil(L, -1))
        return 1;
    lua_pop(L, 1);

    if (!load_chunk(L, module))
        return lua_error(L);
    lua_pushstring(L, module);
    lua_call(L, 1, 1);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        lua_pushboolean(L, 1);
    }
    lua_pushvalue(L, -1);
    lua_setfield(L, -3, module);
    return 1;
}

static int l_save_write(lua_State *L)
{
    size_t len = 0;
    const char *name = luaL_checkstring(L, 1);
    const char *data = luaL_checklstring(L, 2, &len);

    lua_pushboolean(L, savefile_write(name, data, len));
    return 1;
}

static int l_save_read(lua_State *L)
{
    size_t len = 0;
    char *data = savefile_read(luaL_checkstring(L, 1), &len);

    if (data == NULL) {
        lua_pushnil(L);
        return 1;
    }
    lua_pushlstring(L, data, len);
    free(data);
    return 1;
}

static int l_host_command(lua_State *L)
{
    if (g_config.on_command != NULL)
        g_config.on_command(luaL_checkstring(L, 1));
    return 0;
}

/* sys.version(): name, code. Code 0 is a dev build, which never updates. */
static int l_version(lua_State *L)
{
    lua_pushstring(L, LOFTA_VERSION_NAME);
    lua_pushinteger(L, (lua_Integer)LOFTA_VERSION_CODE);
    return 2;
}

/* sys.update_status(): a short state word and the offered version name, for the title screen. */
static int l_update_status(lua_State *L)
{
    char offered[64] = "";
    const char *state = "off";

    if (g_config.update_status != NULL)
        state = g_config.update_status(offered, sizeof offered);
    lua_pushstring(L, state);
    lua_pushstring(L, offered);
    return 2;
}

static const luaL_Reg SYS_FUNCS[] = {
    {"write", l_save_write},
    {"read", l_save_read},
    {"command", l_host_command},
    {"version", l_version},
    {"update_status", l_update_status},
    {NULL, NULL},
};

void lua_host_bind_sys(lua_State *L)
{
    luaL_newlib(L, SYS_FUNCS);
    lua_setglobal(L, "sys");
}

static void remove_field(lua_State *L, const char *table, const char *field)
{
    if (table == NULL) {
        lua_pushnil(L);
        lua_setglobal(L, field);
        return;
    }
    lua_getglobal(L, table);
    lua_pushnil(L);
    lua_setfield(L, -2, field);
    lua_pop(L, 1);
}

/* Scripts get base, string, table, math and utf8, minus everything that reaches the file
 * system or depends on chance. See docs/TECH.md. */
static void open_sandbox(lua_State *L)
{
    luaL_requiref(L, LUA_GNAME, luaopen_base, 1);
    luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
    luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);
    luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 1);
    luaL_requiref(L, LUA_UTF8LIBNAME, luaopen_utf8, 1);
    if (g_config.debug_library)
        luaL_requiref(L, LUA_DBLIBNAME, luaopen_debug, 1);
    lua_pop(L, g_config.debug_library ? 6 : 5);

    remove_field(L, NULL, "dofile");
    remove_field(L, NULL, "loadfile");
    remove_field(L, NULL, "load");
    remove_field(L, "math", "random");
    remove_field(L, "math", "randomseed");
    remove_field(L, "string", "dump");
}

lua_State *lua_host_open(const LuaHostConfig *config)
{
    lua_State *L = luaL_newstate();

    if (L == NULL)
        return NULL;
    g_config = *config;
    open_sandbox(L);

    lua_newtable(L);
    lua_setfield(L, LUA_REGISTRYINDEX, LOADED_KEY);
    lua_register(L, "require", l_require);
    lua_bind_core(L);
    lua_host_bind_sys(L);

    if (!lua_host_load_main(L)) {
        lua_close(L);
        return NULL;
    }
    return L;
}

void lua_host_close(lua_State *L)
{
    if (L != NULL)
        lua_close(L);
}

int lua_host_load_main(lua_State *L)
{
    lua_newtable(L);
    lua_setfield(L, LUA_REGISTRYINDEX, LOADED_KEY);

    lua_getglobal(L, "require");
    lua_pushliteral(L, "main");
    if (!lua_host_pcall(L, 1, 1, "main"))
        return 0;
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        report("main", "main.lua must return a table");
        return 0;
    }
    lua_setglobal(L, "main");
    return 1;
}

static int push_main_fn(lua_State *L, const char *fn)
{
    lua_getglobal(L, "main");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return 0;
    }
    lua_getfield(L, -1, fn);
    lua_remove(L, -2);
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 1);
        return 0;
    }
    return 1;
}

int lua_host_call(lua_State *L, const char *fn)
{
    return push_main_fn(L, fn) && lua_host_pcall(L, 0, 0, fn);
}

int lua_host_call_str(lua_State *L, const char *fn, const char *arg)
{
    if (!push_main_fn(L, fn))
        return 0;
    lua_pushstring(L, arg);
    return lua_host_pcall(L, 1, 0, fn);
}

int lua_host_call_xy(lua_State *L, const char *fn, int x, int y)
{
    if (!push_main_fn(L, fn))
        return 0;
    lua_pushinteger(L, (lua_Integer)x);
    lua_pushinteger(L, (lua_Integer)y);
    return lua_host_pcall(L, 2, 0, fn);
}

/* The table is reused rather than rebuilt, so a screen may keep no reference to it. */
void lua_host_set_pointer(lua_State *L, int x, int y, int inside)
{
    if (L == NULL)
        return;
    if (!inside) {
        lua_pushnil(L);
        lua_setglobal(L, "pointer");
        return;
    }
    lua_getglobal(L, "pointer");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        lua_newtable(L);
    }
    lua_pushinteger(L, (lua_Integer)x);
    lua_setfield(L, -2, "x");
    lua_pushinteger(L, (lua_Integer)y);
    lua_setfield(L, -2, "y");
    lua_setglobal(L, "pointer");
}
