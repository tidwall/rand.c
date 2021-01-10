// Copyright 2021 Joshua J Baker. All rights reserved.
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file.

#include "rand.h"

// Derived from pcg32_random_r (https://www.pcg-random.org/download.html)
// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)
static uint32_t pcg32(uint64_t *state) {
    uint64_t oldstate = *state;
    // Advance internal state
    *state = oldstate * 6364136223846793005ULL + 1;
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

// rand_seed returns a pseudo-random number generator initialized with the 
// provided seed.
struct rand rand_seed(uint64_t seed) {
    struct rand rand = (struct rand){ .state = seed };
    pcg32(&rand.state);
    return rand;
}

// rand_uint32 returns a pseudo-random 32-bit value.
uint32_t rand_uint32(struct rand *rand) {
    return pcg32(&rand->state);
}

// rand_uint64 returns a pseudo-random 64-bit value.
uint64_t rand_uint64(struct rand *rand) {
    return ((uint64_t)rand_uint32(rand)) << 32 | (uint64_t)(rand_uint32(rand));
}

// rand_int63 returns a non-negative pseudo-random 63-bit integer.
int64_t rand_int63(struct rand *rand) {
    return (int64_t)(rand_uint64(rand) >> 1);
}

// rand_int32 returns a non-negative pseudo-random 31-bit integer.
int32_t rand_int31(struct rand *rand) {
    return (int32_t)(rand_uint32(rand) >> 1);
}

// rand_double returns a pseudo-random double in [0.0,1.0).
double rand_double(struct rand *rand) {
    while (1) {
        double f = (double)(rand_int63(rand)) / ((uint64_t)1 << 63);
        if (f < 1.0) {
            return f;
        }
    }
}

// rand_fill writes pseudo-random bytes to data.
void rand_fill(struct rand *rand, void *data, size_t nbytes) {
    size_t i = 0;
    if (nbytes > 8) {
        for (i = 0; i < nbytes-8; i += 8) {
            *(uint64_t*)(((char*)data)+i) = rand_uint64(rand);
        }
    }
    for (; i < nbytes; i++) {
        *(((char*)data)+i) = rand_uint32(rand);
    }
}

//==============================================================================
// TESTS
// $ cc -DRAND_TEST rand.c && ./a.out              # run tests
//==============================================================================
#ifdef RAND_TEST

#include <stdio.h>
#include <inttypes.h>

static void test_all() {
    uint32_t values[] =  {
        2424192242, 1853756841, 3339140693, 332856550, 985935363, 265305501, 
        1342587035, 3378928879, 2116085098, 1102966071, 2581679090, 2585186576, 
        3101274157, 3202861717, 2129991638, 657320098, 333339907, 2631700936, 
        2594965664, 2036117813, 1333641093, 3757086234, 2658997153, 3118844381, 
        2457780294, 1435150030, 1236376865, 3651838824, 1010214129, 1545702616,
        3744313245, 1665861397, 3434192109, 1475777479, 2183880465, 2743788398, 
        2071166281, 610012080, 3438497271, 223755471, 4059206119, 776566956, 
        3720683370, 3438126235, 2046975621, 3697025923, 1886628042, 1228642160, 
        1991252383, 3836563300, 496326019, 3372504206, 1399660180, 2870296435, 
        1999269025, 1674451472, 3885090021, 2972477897, 3327143797, 3958160509, 
        2676142559, 1232377885, 1569552633, 1909034303, 3454749434, 921129889, 
        827760511, 844322347, 527951796, 3920197271, 2837578723, 2281060709, 
        1709962515, 2935779755, 1974279119, 1475613390, 430049498, 1509733982, 
        2268181253, 2489560974, 1995311818, 2616323223, 876360029, 
        21061372, 3757860649, 1145591663, 2009627361, 1337604179, 1281029858, 
        3309063769, 4140788585,669003015, 3591610852, 523817405, 3071878542, 
        1343426242, 2401122788, 1246980383,4173306414,37009700,
    };
    struct rand rand = rand_seed(2010219);
    for (int i = 0; i < sizeof(values)/sizeof(uint32_t); i++) {
        uint32_t x = rand_uint32(&rand);
        if (x != values[i]) {
            fprintf(stderr, "fail %" PRIu32 " != %" PRIu32 "\n", x, values[i]);
        }
    }
    {
    // Initialize and seed a new pseudo-random number generator.
    struct rand rand = rand_seed(2468);

    // create a random latitude and longitude point
    double lat = rand_double(&rand) * 180.0 - 90.0;
    double lon = rand_double(&rand) * 360.0 - 180.0;

    printf("lat=%.8f, lon=%.8f\n", lat, lon);
    }
}

int main() {
    printf("Running rand.c tests...\n");
    test_all();
    printf("PASSED\n");
}

#endif
