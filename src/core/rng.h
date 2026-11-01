/* xorshift32 with one stream per concern, so a cosmetic roll never shifts a story roll. */
#ifndef LOFTA_RNG_H
#define LOFTA_RNG_H

#include <stdint.h>

enum { RNG_WORLD, RNG_STORY, RNG_FX, RNG_STREAMS };

void rng_seed(uint32_t state[RNG_STREAMS], uint32_t seed);
uint32_t rng_next(uint32_t state[RNG_STREAMS], int stream);
/* Uniform in [0, n), n > 0. */
uint32_t rng_range(uint32_t state[RNG_STREAMS], int stream, uint32_t n);

#endif
