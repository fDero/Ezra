
#include "ezra/chacha20.h"

#include <stdio.h>
#include <time.h>

#define SAMPLES_SIZE 30000
#define BUCKETS_SIZE 10

int main(void) {
    ezra_chacha20_t cc;
    uint64_t seed = time(NULL);
    ezra_init_chacha20(seed, &cc);
    double samples[SAMPLES_SIZE];
    
    printf("Generating %d doubles in range (0,1)...\n", SAMPLES_SIZE);
    for (size_t i = 0; i < SAMPLES_SIZE; i++) {
        ezra_random_number_t rand_num = ezra_rand_generate_chacha20(&cc);
        ezra_real_range_t range;
        ezra_init_open_real_range(0.0, 1.0, &range);
        double scaled = ezra_scale_in_real_range(&range, &rand_num);
        samples[i] = scaled;
    }

    uint32_t buckets[BUCKETS_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    printf("Subdiving results in %d buckets\n", BUCKETS_SIZE);
    for (size_t i = 0; i < SAMPLES_SIZE; i++) {
        size_t bucket_id = (size_t) (samples[i] * BUCKETS_SIZE);
        buckets[bucket_id]++;
    }

    size_t max_buckets_count = 0;
    size_t min_buckets_count = SAMPLES_SIZE;
    for (size_t i = 0; i < 10; i++) {
        printf("Bucket %zu contains: %u samples\n", i, buckets[i]);
        max_buckets_count = buckets[i] > max_buckets_count ? buckets[i] : max_buckets_count;
        min_buckets_count = buckets[i] < min_buckets_count ? buckets[i] : min_buckets_count;
    }

    size_t diff = max_buckets_count - min_buckets_count;

    printf("Max samples in a bucket: %zu\n", max_buckets_count);
    printf("Min samples in a bucket: %zu\n", min_buckets_count);
    printf("Difference: %zu\n", diff);

    size_t expected_bucket_size = SAMPLES_SIZE / BUCKETS_SIZE;
    printf("Expected bucket size: %zu\n", expected_bucket_size);

    double chi_squared = 0.0;
    for (size_t i = 0; i < BUCKETS_SIZE; i++) {
        int observed = buckets[i];
        int expected = expected_bucket_size;
        int diff = observed - expected;
        chi_squared += 1.0 * diff * diff / expected;
    }

    printf("Chi-squared: %f\n", chi_squared);
    return 0;
}