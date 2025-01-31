
#include "ezra/chacha20.h"
#include "ezra/mt19937.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define MT19937_ID 1
#define CHACHA20_ID 2

#define RAW_BYTES_ID 1
#define INTS_0_9_ID 2
#define OPEN_REALS_ID 3
#define CLOSED_REALS_ID 4

int8_t ask_for_algo(){
    int8_t algo_choice = 0;
    size_t attempts = 0;
    int scanf_result = 0;

    do {
        if (attempts++ > 0) {
            printf("invalid choice, please try again: \n\n");
        }

        printf("which algorithm do you want to test?\n");
        printf("[%d] mt19937 (64 bit / 312 cell implementation of a Mersenne/Twister) \n", MT19937_ID);
        printf("[%d] chacha20 (PRNG based on the ChaCha20 stream cipher) \n",              CHACHA20_ID);
        
        printf("> ");
        scanf_result = scanf("%hhd", &algo_choice);
        while (getchar() != '\n');

    } while (scanf_result != 1 || algo_choice < 1 || algo_choice > 2);
    printf("\n");

    return algo_choice;
}

int8_t ask_for_out() {
    int8_t out_choice = 0;
    size_t attempts = 0;
    int scanf_result = 0; 

    do {
        if (attempts++ > 0) {
            printf("invalid choice, please try again: \n\n");
        }

        printf("which output do you want?\n");
        printf("[%d] 64bits sequence \n",        RAW_BYTES_ID);
        printf("[%d] integers between [0,9] \n", INTS_0_9_ID);
        printf("[%d] doubles in range (0,1) \n", OPEN_REALS_ID);
        printf("[%d] doubles in range [0,1] \n", CLOSED_REALS_ID);
        
        printf("> ");
        scanf_result = scanf("%hhd", &out_choice);
        while (getchar() != '\n');

    } while (scanf_result != 1 || out_choice < 1 || out_choice > 4);
    printf("\n");

    return out_choice;
}

size_t ask_for_sample_size() {
    size_t sample_size = 0;
    size_t attempts = 0;
    int scanf_result = 0;

    do {
        if (attempts++ > 0) {
            printf("invalid choice, please try again: \n\n");
        }

        printf("how many samples do you want to generate?\n");
        printf("please enter a number (suggested size is at least 30000)\n");
        
        printf("> ");
        scanf_result = scanf("%llu", &sample_size);
        while (getchar() != '\n');

    } while (scanf_result != 1 || sample_size < 1);
    printf("\n");

    return sample_size;
}

uint64_t ask_for_seed() {
    uint64_t seed = 0;
    size_t attempts = 0;

    do {
        if (attempts++ > 0) {
            printf("invalid choice, please try again: \n\n");
        }

        printf("please enter a seed (default:%lld)\n", time(NULL));
        
        printf("> ");
        scanf("%llu", &seed);
        while (getchar() != '\n');
        
    } while (seed < 1);
    printf("\n");

    return seed;
}

FILE* ask_for_file() {
    FILE* file = NULL;
    size_t attempts = 0;

    do {
        if (attempts++ > 0) {
            printf("invalid choice, please try again: \n\n");
        }

        printf("please enter a file name:\n");
        
        printf("> ");
        char file_name[256];
        scanf("%s", file_name);
        file = fopen(file_name, "w");
    } while (file == NULL);
    printf("\n");

    return file;
}

int main() {    
    printf("    ______                  \n");
    printf("   / ____/___  _________ _  \n");
    printf("  / __/ /_  / / ___/ __ `/  \n");
    printf(" / /___  / /_/ /  / /_/ /   \n");
    printf("/_____/ /___/_/   \\__,_/   \n");
    printf("        samples generator\n\n");
    
    int8_t algo_choice = ask_for_algo();
    int8_t out_choice = ask_for_out();
    size_t sample_size = ask_for_sample_size();
    size_t seed = ask_for_seed();
    FILE* file = ask_for_file();
    
    void* generator = NULL;
    ezra_random_number_t (*generate_callback)(void*) = NULL;

    switch (algo_choice) {
        break; case MT19937_ID: {
            generator = malloc(sizeof(ezra_mt19937_t));
            ezra_init_mt19937(seed, (ezra_mt19937_t*)generator);
            generate_callback = (ezra_random_number_t (*)(void *)) ezra_rand_generate_mt19937;
        }
        break; case CHACHA20_ID: {
            generator = malloc(sizeof(ezra_chacha20_t));
            ezra_init_chacha20(seed, (ezra_chacha20_t*)generator);
            generate_callback = (ezra_random_number_t (*)(void *)) ezra_rand_generate_chacha20;
        }
    }

    ezra_int_range_t int_range_0_9;
    ezra_init_int_range(0, 9, &int_range_0_9);

    ezra_real_range_t open_real_range;
    ezra_init_open_real_range(0.0, 1.0, &open_real_range);

    ezra_real_range_t closed_real_range;
    ezra_init_closed_real_range(0.0, 1.0, &closed_real_range);

    for (size_t i = 0; i < sample_size; i++) {
        ezra_random_number_t rand_num = generate_callback(generator);
        switch (out_choice) {
            break; case RAW_BYTES_ID: {
                fprintf(file, "%llx\n", rand_num.integer_number);
            }
            break; case INTS_0_9_ID: {
                fprintf(file, "%lld\n", ezra_scale_in_int_range(&int_range_0_9, &rand_num));
            }
            break; case OPEN_REALS_ID: {
                fprintf(file, "%f\n", ezra_scale_in_real_range(&open_real_range, &rand_num));
            }
            break; case CLOSED_REALS_ID: {
                fprintf(file, "%f\n", ezra_scale_in_real_range(&closed_real_range, &rand_num));
            }
        }
    }

    return 0;
}