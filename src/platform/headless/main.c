/* Headless host: runs a Lua test script through the same scripts and core a player gets, and
 * writes screenshots. Built with ASan and UBSan on Linux. A failed check sets the exit code. */
#include <lauxlib.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "core_api.h"
#include "lua_host.h"
#include "png.h"
#include "savefile.h"

static const char *shot_dir = "build/shots";

static void on_error(const char *context, const char *message)
{
    fprintf(stderr, "%s: %s\n", context, message);
}

static void make_dir(const char *path)
{
#ifdef _WIN32
    _mkdir(path);
#else
    mkdir(path, 0777);
#endif
}

/* host.shot(name, scale): writes the finished frame as a PNG. */
static int l_shot(lua_State *L)
{
    const char *name = luaL_checkstring(L, 1);
    int scale = (int)luaL_optinteger(L, 2, 1);
    char path[256];
    const uint8_t *fb;
    const uint8_t *pal;
    static unsigned char palette[256][3];

    snprintf(path, sizeof path, "%s/%s.png", shot_dir, name);
    core_frame_indexed(&fb, &pal);
    memcpy(palette, pal, sizeof palette);
    if (!png_write_indexed(path, fb, FB_W, FB_H, palette, scale))
        return luaL_error(L, "cannot write %s", path);
    return 0;
}

/* host.report(line): a failed check goes to stderr, as a compiler error would. */
static int l_report(lua_State *L)
{
    fprintf(stderr, "%s\n", luaL_checkstring(L, 1));
    return 0;
}

/* host.print(line): results a tool reads from stdout. */
static int l_print(lua_State *L)
{
    fprintf(stdout, "%s\n", luaL_checkstring(L, 1));
    return 0;
}

/* host.frame_begin(): real hosts do this before main.on_draw. */
static int l_frame_begin(lua_State *L)
{
    (void)L;
    core_frame_begin();
    return 0;
}

static const luaL_Reg HOST_FUNCS[] = {
    {"shot", l_shot},
    {"report", l_report},
    {"print", l_print},
    {"frame_begin", l_frame_begin},
    {NULL, NULL},
};

/* Runs script as a chunk, then returns testlib.finish() as the failure count. */
static int run(lua_State *L, const char *script)
{
    int failures = 1;

    luaL_newlib(L, HOST_FUNCS);
    lua_setglobal(L, "host");

    if (luaL_loadfilex(L, script, "t") != LUA_OK) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        return 1;
    }
    lua_pushstring(L, script);
    if (!lua_host_pcall(L, 1, 0, script))
        return 1;

    lua_getglobal(L, "require");
    lua_pushliteral(L, "testlib");
    if (!lua_host_pcall(L, 1, 1, "testlib"))
        return 1;
    lua_getfield(L, -1, "finish");
    lua_remove(L, -2);
    lua_pushstring(L, script);
    if (!lua_host_pcall(L, 1, 1, "testlib.finish"))
        return 1;
    failures = (int)lua_tointeger(L, -1);
    lua_pop(L, 1);
    return failures == 0 ? 0 : 1;
}

int main(int argc, char **argv)
{
    LuaHostConfig config;
    lua_State *L;
    int status;

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "usage: %s script [shot-dir]\n", argv[0]);
        return 2;
    }
    if (argc == 3)
        shot_dir = argv[2];
    make_dir("build");
    make_dir(shot_dir);
    savefile_init("build/test_save");

    if (core_init(1u) != 0) {
        fprintf(stderr, "bad sprite art\n");
        return 2;
    }

    memset(&config, 0, sizeof config);
    config.dirs[0] = "tests/lib";
    config.dirs[1] = "scripts";
    config.dirs[2] = "build/gen";
    config.debug_library = 1;
    config.on_error = on_error;

    L = lua_host_open(&config);
    if (L == NULL)
        return 2;
    status = run(L, argv[1]);
    lua_host_close(L);
    return status;
}
