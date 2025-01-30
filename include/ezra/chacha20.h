
#include <stdint.h>
#include "ezra/random.h"

#define CHACHA20_BLOCK_SIZE 16
#define CHACHA20_ITERATIONS 20

typedef struct {
    uint32_t input[CHACHA20_BLOCK_SIZE];
    uint32_t output[CHACHA20_BLOCK_SIZE];
    size_t output_index;
} ezra_chacha20_t;


void ezra_init_chacha20(
    const uint64_t seed, 
    ezra_chacha20_t* cc
);


ezra_random_number_t ezra_rand_generate_chacha20(
    ezra_chacha20_t* cc
);