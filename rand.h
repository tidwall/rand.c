// Copyright 2021 Joshua J Baker. All rights reserved.
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file.

#ifndef RAND_H
#define RAND_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct rand {
    uint64_t state;
};

struct rand rand_seed(uint64_t seed);
uint32_t rand_uint32(struct rand *rand);
uint64_t rand_uint64(struct rand *rand);
int64_t rand_int63(struct rand *rand);
int32_t rand_int31(struct rand *rand);
double rand_double(struct rand *rand);
void rand_fill(struct rand *rand, void *data, size_t len);

bool rand_crypto_fill(void *data, size_t len);

#endif
