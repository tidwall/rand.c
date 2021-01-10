# rand.c

A [PCG](https://www.pcg-random.org) based pseudo-random number generator for C.

## Example

```c
#include <stdio.h>
#include "rand.h"

int main() {
    // Initialize and seed a new pseudo-random number generator.
    struct rand rand = rand_seed(2468);

    // Here we'll generate a random latitude and longitude point
    double lat = rand_double(&rand) * 180.0 - 90.0;
    double lon = rand_double(&rand) * 360.0 - 180.0;

    printf("lat=%.8f, lon=%.8f\n", lat, lon);
}

// output:
// lat=18.22369031, lon=41.10615327

```

## Functions

### Basic

```sh
rand_seed      # initialize and seed a new pseudo-random number generator.
rand_uint32    # returns a pseudo-random 32-bit value.
rand_uint64    # returns a pseudo-random 64-bit value.
rand_int63     # returns a non-negative pseudo-random 63-bit integer.
rand_int31     # returns a non-negative pseudo-random 31-bit integer.
rand_double    # returns a pseudo-random double in [0.0,1.0).
rand_fill      # writes pseudo-random bytes to data.

```

## Testing

```sh
$ cc -DRAND_TEST rand.c && ./a.out              # run tests
```

## License

rand.c source code is available under the MIT License.
