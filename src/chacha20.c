
#include "ezra/chacha20.h"

#define CHACHA20_INIT_STATE_CELL_0 0x61707865
#define CHACHA20_INIT_STATE_CELL_1 0x3320646e
#define CHACHA20_INIT_STATE_CELL_2 0x79622d32
#define CHACHA20_INIT_STATE_CELL_3 0x6b206574
#define SEED_ALTERATION_FACTOR 1103515245
#define SEED_ALTERATION_INCREMENT 12345
#define CHACHA20_COUNTER_INDEX 12

void ezra_init_chacha20(
    const uint64_t seed, 
    ezra_chacha20_t* cc
) {
    cc->output_index = CHACHA20_BLOCK_SIZE;
    cc->input[0] = CHACHA20_INIT_STATE_CELL_0;
    cc->input[1] = CHACHA20_INIT_STATE_CELL_1;
    cc->input[2] = CHACHA20_INIT_STATE_CELL_2;
    cc->input[3] = CHACHA20_INIT_STATE_CELL_3;
    uint32_t random_initial_value = seed;
    for (size_t i = 4; i < CHACHA20_BLOCK_SIZE; i++) {
        random_initial_value *= SEED_ALTERATION_FACTOR;
        random_initial_value += SEED_ALTERATION_INCREMENT;
        cc->input[i] = random_initial_value;
    }
    cc->input[CHACHA20_COUNTER_INDEX] = 0;    
}

static uint32_t ezra_chacha_rotl32(uint32_t rotate_me, const size_t amount) {
    uint32_t rotated = (rotate_me << amount);
    rotated |= (rotate_me >> (32 - amount));
    return rotated;
}

static void ezra_chacha_qr(
    uint32_t seq[CHACHA20_BLOCK_SIZE], 
    const size_t a, 
    const size_t b, 
    const size_t c, 
    const size_t d
) {
    seq[a] += seq[b]; seq[d] ^= seq[a]; seq[d] = ezra_chacha_rotl32(seq[d], 16);
    seq[c] += seq[d]; seq[b] ^= seq[c]; seq[b] = ezra_chacha_rotl32(seq[b], 12);
    seq[a] += seq[b]; seq[d] ^= seq[a]; seq[d] = ezra_chacha_rotl32(seq[d], 8);
    seq[c] += seq[d]; seq[b] ^= seq[c]; seq[b] = ezra_chacha_rotl32(seq[b], 7);
}

static void ezra_chacha20_next_block(ezra_chacha20_t* cc) {
	for (size_t i = 0; i < CHACHA20_BLOCK_SIZE; ++i) {
		cc->output[i] = cc->input[i];
    }
	for (size_t i = 0; i < CHACHA20_ITERATIONS / 2 ; i += 2) {
		
		ezra_chacha_qr(cc->output, 0, 4,  8, 12);
		ezra_chacha_qr(cc->output, 1, 5,  9, 13);
		ezra_chacha_qr(cc->output, 2, 6, 10, 14);
		ezra_chacha_qr(cc->output, 3, 7, 11, 15);
		
        ezra_chacha_qr(cc->output, 0, 5, 10, 15);
		ezra_chacha_qr(cc->output, 1, 6, 11, 12);
		ezra_chacha_qr(cc->output, 2, 7,  8, 13);
		ezra_chacha_qr(cc->output, 3, 4,  9, 14);
	}
	for (size_t i = 0; i < CHACHA20_BLOCK_SIZE; ++i) {
		cc->output[i] += cc->input[i];
    }
    cc->output_index = 0;
    cc->input[CHACHA20_COUNTER_INDEX]++;
}

ezra_random_number_t ezra_rand_generate_chacha20(
    ezra_chacha20_t* cc
) {
    if (cc->output_index >= CHACHA20_BLOCK_SIZE) {
        ezra_chacha20_next_block(cc);
    }
    uint32_t first_partial_output = cc->output[cc->output_index++];
    uint32_t second_partial_output = cc->output[cc->output_index++];
    uint64_t output = first_partial_output | ((uint64_t) second_partial_output << 32);
    ezra_random_number_t random_number = { output }; 
    return random_number;
}