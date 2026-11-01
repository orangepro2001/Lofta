#include "path.h"

#include <string.h>

#include "state.h"

#define NODES (SCENE_MAX_W * SCENE_MAX_H)
#define COST_STRAIGHT 10
#define COST_DIAGONAL 14

const int8_t PATH_DX[8] = {1, 1, 0, -1, -1, -1, 0, 1};
const int8_t PATH_DY[8] = {0, 1, 1, 1, 0, -1, -1, -1};

static uint16_t g_cost[NODES];
static uint8_t g_from[NODES];
static uint8_t g_closed[NODES];
static uint16_t g_heap[NODES];
static uint32_t g_heap_key[NODES];
static int g_heap_size;

static int node_of(int x, int y)
{
    return y * SCENE_MAX_W + x;
}

static int key_less(int a, int b)
{
    return g_heap_key[a] < g_heap_key[b];
}

static void heap_swap(int a, int b)
{
    uint16_t n = g_heap[a];
    uint32_t k = g_heap_key[a];

    g_heap[a] = g_heap[b];
    g_heap_key[a] = g_heap_key[b];
    g_heap[b] = n;
    g_heap_key[b] = k;
}

static void heap_push(int node, uint32_t key)
{
    int i = g_heap_size++;

    g_heap[i] = (uint16_t)node;
    g_heap_key[i] = key;
    while (i > 0) {
        int parent = (i - 1) / 2;

        if (!key_less(i, parent))
            break;
        heap_swap(i, parent);
        i = parent;
    }
}

static int heap_pop(void)
{
    int top = g_heap[0];
    int i = 0;

    g_heap_size--;
    g_heap[0] = g_heap[g_heap_size];
    g_heap_key[0] = g_heap_key[g_heap_size];
    for (;;) {
        int left = i * 2 + 1;
        int right = left + 1;
        int best = i;

        if (left < g_heap_size && key_less(left, best))
            best = left;
        if (right < g_heap_size && key_less(right, best))
            best = right;
        if (best == i)
            break;
        heap_swap(i, best);
        i = best;
    }
    return top;
}

static int abs_int(int v)
{
    return v < 0 ? -v : v;
}

static int cheb(int x, int y, int tx, int ty)
{
    int dx = abs_int(x - tx);
    int dy = abs_int(y - ty);

    return dx > dy ? dx : dy;
}

static int estimate(int x, int y, int tx, int ty)
{
    int dx = abs_int(x - tx);
    int dy = abs_int(y - ty);
    int low = dx < dy ? dx : dy;
    int high = dx < dy ? dy : dx;

    return low * COST_DIAGONAL + (high - low) * COST_STRAIGHT;
}

int path_find(int sx, int sy, int tx, int ty, int stop, uint8_t *out, int cap)
{
    int start, goal = -1, serial = 0;

    if (!state_walkable(sx, sy))
        return -1;
    if (cheb(sx, sy, tx, ty) <= stop)
        return 0;

    memset(g_cost, 0xFF, sizeof g_cost);
    memset(g_closed, 0, sizeof g_closed);
    g_heap_size = 0;
    start = node_of(sx, sy);
    g_cost[start] = 0;
    /* serial breaks ties the same way on every compiler */
    heap_push(start, ((uint32_t)estimate(sx, sy, tx, ty) << 16) | (uint32_t)serial++);

    while (g_heap_size > 0) {
        int node = heap_pop();
        int x = node % SCENE_MAX_W;
        int y = node / SCENE_MAX_W;
        int d;

        if (g_closed[node])
            continue;
        g_closed[node] = 1;
        if (cheb(x, y, tx, ty) <= stop) {
            goal = node;
            break;
        }
        for (d = 0; d < 8; d++) {
            int nx = x + PATH_DX[d];
            int ny = y + PATH_DY[d];
            int diagonal = (d & 1) != 0;
            int cost, next;

            if (!state_walkable(nx, ny))
                continue;
            if (diagonal && (!state_walkable(x + PATH_DX[d], y) || !state_walkable(x, y + PATH_DY[d])))
                continue;
            next = node_of(nx, ny);
            cost = g_cost[node] + (diagonal ? COST_DIAGONAL : COST_STRAIGHT);
            if (g_closed[next] || cost >= g_cost[next])
                continue;
            g_cost[next] = (uint16_t)cost;
            g_from[next] = (uint8_t)d;
            heap_push(next, ((uint32_t)(cost + estimate(nx, ny, tx, ty)) << 16) |
                                (uint32_t)(serial++ & 0xFFFF));
        }
    }

    if (goal < 0)
        return -1;

    {
        int length = 0;
        int node = goal;
        int i;

        while (node != start) {
            int d = g_from[node];

            if (length >= cap)
                return -1;
            out[length++] = (uint8_t)d;
            node -= node_of(PATH_DX[d], PATH_DY[d]);
        }
        for (i = 0; i < length / 2; i++) {
            uint8_t t = out[i];

            out[i] = out[length - 1 - i];
            out[length - 1 - i] = t;
        }
        return length;
    }
}
