#include "rng.h"

/* xorshift has a fixed point at zero, so every seed is mixed away from it. */
static uint32_t mix_seed(uint32_t seed, uint32_t stream)
{
    uint32_t x = seed ^ (0x9E3779B9u * (stream + 1u));

    x ^= x >> 16;
    x *= 0x7FEB352Du;
    x ^= x >> 15;
    x *= 0x846CA68Bu;
    x ^= x >> 16;
    return x != 0u ? x : 0xA5A5A5A5u;
}

void rng_seed(uint32_t state[RNG_STREAMS], uint32_t seed)
{
    uint32_t i;

    for (i = 0; i < RNG_STREAMS; i++)
        state[i] = mix_seed(seed, i);
}

uint32_t rng_next(uint32_t state[RNG_STREAMS], int stream)
{
    uint32_t x = state[stream];

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state[stream] = x;
    return x;
}

uint32_t rng_range(uint32_t state[RNG_STREAMS], int stream, uint32_t n)
{
    uint32_t limit = 0xFFFFFFFFu - (0xFFFFFFFFu % n);
    uint32_t x;

    do {
        x = rng_next(state, stream);
    } while (x >= limit);
    return x % n;
}
