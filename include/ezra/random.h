
#ifndef EZRA_RANDOM_H
#define EZRA_RANDOM_H

#include <stdint.h>

typedef struct {
    uint64_t integer_number;
} ezra_random_number_t;


typedef struct {
    double lower_bound;
    double upper_bound;
    int8_t lower_bound_inclusive;
    int8_t upper_bound_inclusive;
} ezra_real_range_t;


typedef struct {
    int64_t lower_bound;
    int64_t upper_bound;
} ezra_int_range_t;


void ezra_init_open_real_range(
    double lower_bound, 
    double upper_bound, 
    ezra_real_range_t* range 
);


void ezra_init_open_real_range(
    double lower_bound, 
    double upper_bound, 
    ezra_real_range_t* range 
);


void ezra_init_closed_real_range(
    double lower_bound, 
    double upper_bound, 
    ezra_real_range_t* range 
);


void ezra_init_int_range(
    int64_t lower_bound, 
    int64_t upper_bound, 
    ezra_int_range_t* range 
);

double ezra_scale_in_real_range(
    ezra_real_range_t const * const range,
    ezra_random_number_t const * const rand_num
);


int64_t ezra_scale_in_int_range(
    ezra_int_range_t const * const range,
    ezra_random_number_t const * const rand_num
);

#endif