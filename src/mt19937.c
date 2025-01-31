
#include "ezra/mt19937.h"

#include <stdio.h>
#include <stdint.h>


static uint64_t ezra_grow_seed_mt19937(const uint64_t seed) {
    uint64_t incrementally_grown_seed = seed;
    for (size_t i = 0; i < 50; i++) {
        incrementally_grown_seed *= 69069;
        incrementally_grown_seed += 1;
    } 
    return incrementally_grown_seed;
}


void ezra_init_mt19937(const uint64_t seed, ezra_mt19937_t* mt) {
    mt->values[0] = ezra_grow_seed_mt19937(seed);
    for (mt->cursor = 1; mt->cursor < EZRA_MT19937_SIZE; mt->cursor++) {
        size_t i = mt->cursor;
        mt->values[i] = (mt->values[i-1] ^ (mt->values[i-1] >> 62));
        mt->values[i] *= 6364136223846793005ULL;
        mt->values[i] += (uint64_t) i;
    }
}


static void ezra_twist_mt19937(ezra_mt19937_t* mt) {
    uint64_t magic_numbers[2] = {0ULL, 0xB5026F5AA96619E9ULL};
    uint64_t most_significant_33_bits = 0xFFFFFFFF80000000ULL;
    uint64_t least_significant_31_bits = 0x7FFFFFFFULL;
    uint64_t half_of_EZRA_MT19937_SIZE = EZRA_MT19937_SIZE / 2;
    for (size_t i = 0; i < EZRA_MT19937_SIZE; i++) {
        uint64_t tmp = (mt->values[i] & most_significant_33_bits);
        tmp |= (mt->values[(i+1) % EZRA_MT19937_SIZE] & least_significant_31_bits);
        size_t branchless_condition = i >= (EZRA_MT19937_SIZE - half_of_EZRA_MT19937_SIZE);
        size_t offset = half_of_EZRA_MT19937_SIZE - (EZRA_MT19937_SIZE * branchless_condition);
        mt->values[i] = mt->values[i + offset];
        mt->values[i] ^= (tmp >> 1) ^ magic_numbers[tmp & 1ULL];
    }
    mt->cursor = 0;
}


static uint64_t ezra_temper_mt19937(const uint64_t original_value) {
    uint64_t out = original_value;
    out ^= (out >> 29) & 0x5555555555555555ULL;
    out ^= (out << 17) & 0x71D67FFFEDA60000ULL;
    out ^= (out << 37) & 0xFFF7EEE000000000ULL;
    out ^= (out >> 43);
    return out;
}


ezra_random_number_t ezra_rand_generate_mt19937(ezra_mt19937_t* mt) {
    if (mt->cursor >= EZRA_MT19937_SIZE) {
        ezra_twist_mt19937(mt);
    }
    uint64_t selected_value = mt->values[mt->cursor++];
    uint64_t tempered_value = ezra_temper_mt19937(selected_value);
    ezra_random_number_t random_number = { tempered_value };
    return random_number;
}