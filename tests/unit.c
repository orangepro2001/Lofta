/* Core tests that need no Lua. */
#include <stdio.h>
#include <string.h>

#include "core_api.h"
#include "iso.h"
#include "path.h"
#include "rng.h"
#include "state.h"
#include "text.h"

static int g_failed;
static int g_checks;

#define CHECK(cond)                                                     \
    do {                                                                \
        g_checks++;                                                     \
        if (!(cond)) {                                                  \
            g_failed++;                                                 \
            fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #cond);  \
        }                                                               \
    } while (0)

static void test_rng(void)
{
    uint32_t a[RNG_STREAMS], b[RNG_STREAMS];
    int i;

    rng_seed(a, 42u);
    rng_seed(b, 42u);
    for (i = 0; i < 1000; i++)
        CHECK(rng_next(a, RNG_STORY) == rng_next(b, RNG_STORY));
    /* Drawing from one stream leaves the others alone. */
    rng_seed(a, 42u);
    rng_seed(b, 42u);
    rng_next(a, RNG_FX);
    CHECK(rng_next(a, RNG_STORY) == rng_next(b, RNG_STORY));
    for (i = 0; i < 1000; i++)
        CHECK(rng_range(a, RNG_WORLD, 7u) < 7u);
    rng_seed(a, 0u);
    CHECK(a[0] != 0u && a[1] != 0u && a[2] != 0u);
}

static void test_iso(void)
{
    int x, y;

    /* Every cell center survives the trip to the screen and back. */
    for (y = -40; y < 40; y++) {
        for (x = -40; x < 40; x++) {
            int32_t fx = x * CELL_FP + CELL_FP / 2;
            int32_t fy = y * CELL_FP + CELL_FP / 2;
            int32_t wx, wy, rx, ry;

            iso_project(fx, fy, &wx, &wy);
            iso_unproject(wx, wy, &rx, &ry);
            CHECK(rx >> 8 == x);
            CHECK(ry >> 8 == y);
        }
    }
}

static void test_path(void)
{
    uint8_t route[ACTOR_PATH_MAX];
    int x, y, length;

    core_init(1u);
    core_scene_begin(12, 12);
    for (y = 0; y < 12; y++)
        for (x = 0; x < 12; x++)
            core_scene_cell(x, y, 1, 0, 0, 0, 0, 0);
    /* A wall with one gap at the far end. */
    for (y = 0; y < 11; y++)
        core_scene_cell(6, y, 1, 4, 1, 0, 0, 0);

    length = path_find(1, 1, 10, 1, 0, route, ACTOR_PATH_MAX);
    CHECK(length > 0);
    {
        int cx = 1, cy = 1, i;

        for (i = 0; i < length; i++) {
            cx += PATH_DX[route[i]];
            cy += PATH_DY[route[i]];
            CHECK(core_cell_walkable(cx, cy));
        }
        CHECK(cx == 10 && cy == 1);
    }
    CHECK(path_find(1, 1, 6, 5, 0, route, ACTOR_PATH_MAX) == -1); /* wall target */
    CHECK(path_find(1, 1, 6, 5, 1, route, ACTOR_PATH_MAX) > 0);   /* beside it */
    CHECK(path_find(1, 1, 1, 1, 0, route, ACTOR_PATH_MAX) == 0);

    /* Sealing the gap leaves no route, and a short buffer refuses instead of overrunning. */
    core_scene_cell(6, 11, 1, 4, 1, 0, 0, 0);
    CHECK(path_find(1, 1, 10, 1, 0, route, ACTOR_PATH_MAX) == -1);
    core_scene_cell(6, 11, 1, 0, 0, 0, 0, 0);
    CHECK(path_find(1, 1, 10, 1, 0, route, 3) == -1);

    /* A diagonal may not cut a wall's corner. */
    core_scene_begin(3, 3);
    for (y = 0; y < 3; y++)
        for (x = 0; x < 3; x++)
            core_scene_cell(x, y, 1, 0, 0, 0, 0, 0);
    core_scene_cell(1, 0, 1, 4, 1, 0, 0, 0);
    length = path_find(0, 0, 1, 1, 0, route, ACTOR_PATH_MAX);
    CHECK(length == 2);
}

static void test_walk(void)
{
    int x, y, id, i;

    core_init(1u);
    core_scene_begin(10, 10);
    for (y = 0; y < 10; y++)
        for (x = 0; x < 10; x++)
            core_scene_cell(x, y, 1, 0, 0, 0, 0, 0);
    id = core_actor_spawn(core_outfit_kind("searcher"), 1, 1, 8);
    CHECK(id >= 0);
    CHECK(core_actor_walk(id, 8, 1, 0) > 0);
    for (i = 0; i < 60; i++)
        core_tick();
    /* A new order mid-walk finishes the step under way, and stays on whole cells. */
    CHECK(core_actor_moving(id));
    CHECK(core_actor_walk(id, 1, 8, 0) > 0);
    for (i = 0; i < 2000 && core_actor_moving(id); i++)
        core_tick();
    core_actor_cell(id, &x, &y);
    CHECK(x == 1 && y == 8);
    CHECK(!core_actor_moving(id));
}

static void test_state(void)
{
    static uint8_t copy[sizeof(CoreState)];
    size_t size = 0;
    const void *bytes;
    uint64_t hash;

    core_init(9u);
    core_scene_begin(4, 4);
    bytes = core_state_bytes(&size);
    CHECK(size == sizeof(CoreState));
    memcpy(copy, bytes, size);
    hash = core_state_hash();
    core_tick();
    CHECK(core_state_hash() != hash);
    CHECK(core_state_load(copy, size));
    CHECK(core_state_hash() == hash);
    CHECK(!core_state_load(copy, size - 1));
    copy[0] ^= 0xFF;
    CHECK(!core_state_load(copy, size));
    CHECK(core_state_hash() == hash);
}

static void test_names(void)
{
    CHECK(core_ground_kind("cobble") > 0);
    CHECK(core_ground_kind("nonsense") == -1);
    CHECK(core_wall_kind("brick") > 0);
    CHECK(core_prop_kind("lamp") > 0);
    CHECK(core_outfit_kind("searcher") >= 0);
    CHECK(core_color("paper") > 0);
}

static void test_text(void)
{
    CHECK(text_length("abc") == 3);
    CHECK(text_length("\xE4\xB8\x80") == 1);
    /* A broken sequence counts each bad byte and still ends. */
    CHECK(text_length("\xE4\xB8") == 2);
    CHECK(text_length("\xFF") == 1);
    CHECK(text_box(0, 0, 100, "", -1, -1) == 0);
    CHECK(text_box(0, 0, 100, "a\nb", -1, -1) == 2);
    /* Wrapping: 0xFF0C (fullwidth comma) may not start a line and 0x300A (open book title) may
     * not end one, so each hands one character to the next line. Advances come from the font,
     * which here holds at least these. */
    CHECK(text_first_line("\xE4\xB8\x80\xE4\xB8\x80\xE4\xB8\x80\xE4\xB8\x80\xE4\xB8\x80\xEF\xBC\x8C", 60) == 4);
    CHECK(text_first_line("\xE4\xB8\x80\xE4\xB8\x80\xE4\xB8\x80\xE4\xB8\x80\xE3\x80\x8A\xE4\xB8\x80", 60) == 4);
    CHECK(text_first_line("\xE4\xB8\x80\xE4\xB8\x80\xE4\xB8\x80\xE4\xB8\x80\xE4\xB8\x80\xE4\xB8\x80", 60) == 5);
    /* A width smaller than one glyph still makes progress. */
    CHECK(text_box(0, 0, 1, "abc", -1, -1) == 3);
}

int main(void)
{
    test_rng();
    test_iso();
    test_path();
    test_walk();
    test_state();
    test_names();
    test_text();
    printf("unit: %d checks, %d failed\n", g_checks, g_failed);
    return g_failed != 0;
}
