/* The real window: scales the framebuffer up, turns input into calls on scripts/main.lua and
 * keeps the fixed tick. Everything else lives in the scripts and the core. */
#include "raylib.h"

#include <lauxlib.h>
#include <lua.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core_api.h"
#include "lua_host.h"
#include "savefile.h"
#include "update.h"
#include "version.h"
#include "viewport.h"

#define FIXED_DT (1.0 / 60.0)
#define MAX_TICKS_PER_LOOP 5
#define DT_SNAP_EPSILON 0.0005
#define ERROR_LOG "lofta_error.txt"

static unsigned char g_rgba[CORE_RGBA_BYTES];
static int g_quit;

static void on_error(const char *context, const char *message)
{
    FILE *f = fopen(ERROR_LOG, "a");

    if (f != NULL) {
        fprintf(f, "%s: %s\n", context, message);
        fclose(f);
    }
}

static void on_command(const char *command)
{
    if (strcmp(command, "quit") == 0)
        g_quit = 1;
    else if (strcmp(command, "fullscreen") == 0)
        ToggleBorderlessWindowed();
    else if (strcmp(command, "update_install") == 0) {
        update_request_install();
        g_quit = 1;
    }
}

typedef struct {
    int key;
    const char *name;
} KeyName;

static const KeyName KEY_NAMES[] = {
    {KEY_LEFT, "left"},    {KEY_RIGHT, "right"}, {KEY_UP, "up"},     {KEY_DOWN, "down"},
    {KEY_A, "a"},          {KEY_D, "d"},         {KEY_W, "w"},       {KEY_S, "s"},
    {KEY_E, "e"},          {KEY_C, "c"},         {KEY_J, "j"},       {KEY_M, "m"},
    {KEY_Q, "q"},          {KEY_R, "r"},         {KEY_ONE, "1"},     {KEY_TWO, "2"},
    {KEY_THREE, "3"},      {KEY_FOUR, "4"},      {KEY_FIVE, "5"},    {KEY_SIX, "6"},
    {KEY_SEVEN, "7"},      {KEY_EIGHT, "8"},     {KEY_NINE, "9"},    {KEY_ESCAPE, "escape"},
    {KEY_ENTER, "return"}, {KEY_SPACE, "space"}, {KEY_TAB, "tab"},
};

static void handle_input(lua_State *L)
{
    size_t i;

    for (i = 0; i < sizeof KEY_NAMES / sizeof KEY_NAMES[0]; i++) {
        if (IsKeyPressed(KEY_NAMES[i].key))
            lua_host_call_str(L, "on_key_down", KEY_NAMES[i].name);
        if (IsKeyReleased(KEY_NAMES[i].key))
            lua_host_call_str(L, "on_key_up", KEY_NAMES[i].name);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Viewport v = viewport_fit(GetScreenWidth(), GetScreenHeight());
        int fx = 0, fy = 0;

        if (viewport_to_fb(&v, GetMouseX(), GetMouseY(), &fx, &fy))
            lua_host_call_xy(L, "on_tap", fx, fy);
    }
}

static void publish_pointer(lua_State *L)
{
    Viewport v = viewport_fit(GetScreenWidth(), GetScreenHeight());
    int fx = 0, fy = 0;

    if (viewport_to_fb(&v, GetMouseX(), GetMouseY(), &fx, &fy))
        lua_host_set_pointer(L, fx, fy, 1);
    else
        lua_host_set_pointer(L, 0, 0, 0);
}

/* The largest whole multiple of the framebuffer that leaves room for the desktop. */
static void fit_window_to_monitor(void)
{
    int monitor = GetCurrentMonitor();
    int mw = GetMonitorWidth(monitor);
    int mh = GetMonitorHeight(monitor);
    int scale = (mw * 9 / 10) / FB_W;

    if ((mh * 9 / 10) / FB_H < scale)
        scale = (mh * 9 / 10) / FB_H;
    if (scale < 1)
        scale = 1;
    SetWindowSize(FB_W * scale, FB_H * scale);
    SetWindowPosition((mw - FB_W * scale) / 2, (mh - FB_H * scale) / 2);
}

static void redraw(lua_State *L, Texture2D texture)
{
    core_frame_begin();
    lua_host_call(L, "on_draw");
    core_frame_rgba(g_rgba);
    UpdateTexture(texture, g_rgba);
}

/* The test hook: frames to run before leaving, and the PNG the last one becomes. */
typedef struct {
    int frames;
    const char *shot;
    const char *result;
} Smoke;

static void finish_smoke(const Smoke *smoke, int frames, int focused)
{
    if (smoke->shot != NULL) {
        Image image = {g_rgba, FB_W, FB_H, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

        ExportImage(image, smoke->shot);
    }
    if (smoke->result != NULL) {
        FILE *f = fopen(smoke->result, "w");

        if (f != NULL) {
            fprintf(f, "frames=%d focused=%d state=%016llx frame=%016llx version=%s code=%d\n", frames, focused,
                    (unsigned long long)core_state_hash(), (unsigned long long)core_frame_hash(),
                    LOFTA_VERSION_NAME, LOFTA_VERSION_CODE);
            fclose(f);
        }
    }
}

int main(int argc, char **argv)
{
    LuaHostConfig config;
    lua_State *L;
    Texture2D texture;
    Image image;
    Smoke smoke = {0, NULL, NULL};
    const char *save_dir = NULL;
    double last;
    double accumulator = 0.0;
    long frames = 0;
    int i;

    /* Handled before any window exists: a headless self-update must not open one. */
    {
        int handled = update_run_command_line(argc, argv);

        if (handled >= 0)
            return handled;
    }

    memset(&config, 0, sizeof config);
    config.on_error = on_error;
    config.on_command = on_command;
    config.update_status = update_status;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--script-dir") == 0 && i + 1 < argc)
            config.dirs[0] = argv[++i];
        else if (strcmp(argv[i], "--save-dir") == 0 && i + 1 < argc)
            save_dir = argv[++i];
        else if (strcmp(argv[i], "--smoke-frames") == 0 && i + 1 < argc)
            smoke.frames = atoi(argv[++i]);
        else if (strcmp(argv[i], "--smoke-shot") == 0 && i + 1 < argc)
            smoke.shot = argv[++i];
        else if (strcmp(argv[i], "--smoke-result") == 0 && i + 1 < argc)
            smoke.result = argv[++i];
    }

    savefile_init(save_dir);
    if (core_init((unsigned int)time(NULL)) != 0)
        return 1;

    L = lua_host_open(&config);
    if (L == NULL)
        return 1;
    lua_host_call(L, "on_start");

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(FB_W * 2, FB_H * 2, "Lofta");
    SetExitKey(KEY_NULL);
    SetTargetFPS(0);
    SetWindowMinSize(FB_W, FB_H);
    fit_window_to_monitor();

    image = GenImageColor(FB_W, FB_H, BLANK);
    texture = LoadTextureFromImage(image);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    UnloadImage(image);

    if (smoke.frames == 0)
        update_start_background();

    last = GetTime();
    while (!WindowShouldClose() && !g_quit) {
        int smoking = smoke.frames > 0;
        int running = smoking || IsWindowFocused();
        double now = GetTime();
        double elapsed = now - last;

        last = now;
        handle_input(L);
        publish_pointer(L);

        if (IsKeyPressed(KEY_F11))
            ToggleBorderlessWindowed();
        if (config.dirs[0] != NULL && IsKeyPressed(KEY_F5))
            lua_host_load_main(L);

        if (elapsed > FIXED_DT - DT_SNAP_EPSILON && elapsed < FIXED_DT + DT_SNAP_EPSILON)
            elapsed = FIXED_DT;
        /* Losing focus pauses the game; time away is not made up afterwards. */
        accumulator = running ? accumulator + elapsed : 0.0;
        if (accumulator > FIXED_DT * MAX_TICKS_PER_LOOP)
            accumulator = FIXED_DT * MAX_TICKS_PER_LOOP;
        /* Nearest whole tick, not the floor, so a display a hair off 60 Hz keeps one tick per
         * frame instead of alternating between zero and two. */
        while (accumulator >= FIXED_DT / 2.0) {
            lua_host_call(L, "on_tick");
            core_tick();
            accumulator -= FIXED_DT;
        }

        redraw(L, texture);

        BeginDrawing();
        ClearBackground(BLACK);
        {
            Viewport v = viewport_fit(GetScreenWidth(), GetScreenHeight());
            Rectangle src = {0.0f, 0.0f, (float)FB_W, (float)FB_H};
            Rectangle dst = {(float)v.x, (float)v.y, (float)v.w, (float)v.h};

            DrawTexturePro(texture, src, dst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        }
        EndDrawing();

        frames++;
        if (smoking && frames >= smoke.frames) {
            finish_smoke(&smoke, (int)frames, IsWindowFocused());
            break;
        }
    }

    update_shutdown();
    UnloadTexture(texture);
    CloseWindow();
    lua_host_close(L);
    return update_finish_install();
}
