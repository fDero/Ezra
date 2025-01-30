
#include "ezra/chacha20.h"

#include <stdio.h>
#include <time.h>

int main(void) {
    ezra_chacha20_t cc;
    uint64_t seed = time(NULL);
    ezra_init_chacha20(seed, &cc);
    
    printf("Random doubles in range (0,1):\n");
    for (size_t i = 0; i < 10; i++) {
        ezra_random_number_t rand_num = ezra_rand_generate_chacha20(&cc);
        ezra_real_range_t range;
        ezra_init_open_real_range(0.0, 1.0, &range);
        double scaled = ezra_scale_in_real_range(&range, &rand_num);
        printf("%f\n", scaled);
    }

    printf("\nRandom integers in range [0,9]:\n");
    for (size_t i = 0; i < 10; i++) {
        ezra_random_number_t rand_num = ezra_rand_generate_chacha20(&cc);
        ezra_int_range_t range;
        ezra_init_int_range(0, 9, &range);
        int64_t scaled = ezra_scale_in_int_range(&range, &rand_num);
        printf("%lld\n", scaled);
    }

    return 0;
}