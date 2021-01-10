#ifndef RAND_H
#define RAND_H

#include <stddef.h>
#include <stdint.h>

struct rand {
    uint64_t state;
};

struct rand rand_seed(uint64_t seed);
uint32_t rand_uint32(struct rand *rand);
uint64_t rand_uint64(struct rand *rand);
int64_t rand_int63(struct rand *rand);
int32_t rand_int31(struct rand *rand);
double rand_double(struct rand *rand);
float rand_float(struct rand *rand);
void rand_fill(struct rand *rand, void *data, size_t len);

#endif
