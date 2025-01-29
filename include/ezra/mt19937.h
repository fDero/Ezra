
#include <stdint.h>
#include "ezra/random.h"

#define mt19937_SIZE 312

typedef struct {
    uint64_t values[mt19937_SIZE];
    size_t cursor;
} ezra_mt19937_t;


void ezra_init_mt19937(
    const uint64_t seed, 
    ezra_mt19937_t* mt
);


ezra_random_number_t ezra_rand_generate_mt19937(
    ezra_mt19937_t* mt
);