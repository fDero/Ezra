
#include "ezra/random.h"

#include <stdint.h>
#include <math.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))


void ezra_init_open_real_range(
    double lower_bound, 
    double upper_bound, 
    ezra_real_range_t* range 
) {
    range->lower_bound = MIN(lower_bound, upper_bound);
    range->upper_bound = MAX(lower_bound, upper_bound);
    range->lower_bound_inclusive = 0;
    range->upper_bound_inclusive = 0;
}


void ezra_init_closed_real_range(
    double lower_bound, 
    double upper_bound, 
    ezra_real_range_t* range 
) {
    range->lower_bound = MIN(lower_bound, upper_bound);
    range->upper_bound = MAX(lower_bound, upper_bound);
    range->lower_bound_inclusive = 1;
    range->upper_bound_inclusive = 1;
}


void ezra_init_int_range(
    int64_t lower_bound, 
    int64_t upper_bound, 
    ezra_int_range_t* range 
) {
    range->lower_bound = MIN(lower_bound, upper_bound);
    range->upper_bound = MAX(lower_bound, upper_bound);
}


double ezra_scale_in_real_range(
    ezra_real_range_t const * const range,
    ezra_random_number_t const * const rand_num
) {
    double epsilon = 1.0 / (1ULL << 53);
    double scaled_0_1 = epsilon * (rand_num->integer_number >> 11); 
    if (!range->upper_bound_inclusive) {
        scaled_0_1 *= (1.0 - epsilon);
    }
    if (!range->lower_bound_inclusive) {
        scaled_0_1 *= (1.0 - epsilon);
        scaled_0_1 += epsilon;
    }
    double delta = range->upper_bound - range->lower_bound;
    return range->lower_bound + (scaled_0_1 * delta);
}


int64_t ezra_scale_in_int_range(
    ezra_int_range_t const * const range,
    ezra_random_number_t const * const rand_num
) {
    int64_t delta = range->upper_bound - range->lower_bound + 1;
    return rand_num->integer_number % delta + range->lower_bound;
}