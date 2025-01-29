
# Ezra: Easy Randomness

Ezra is a minimal and lightweight C library for 
pseudo-random number generation with no third-party
dependencies.

### Understand the API
The API is simple, and revolves around the 
`ezra_random_number_t` type defined in `<ezra/random.h>`.

You can generate a `ezra_random_number_t` using 
a generator of your choice, and you can extract 
either a `double` or an `int32_t` from it in a given
range.

For instance, assume you want to use 
an [mt19937](https://en.wikipedia.org/wiki/Mersenne_Twister) 
generator (Mersenne / Twister generator), 
you have to setup the generator first:
```c
ezra_mt19937_t mt;
uint64_t seed = time(NULL);
ezra_init_mt19937(seed, &mt);
```

Then you can use the generator to generate 
an `ezra_random_number_t`.
```c
ezra_random_number_t rand_num = ezra_rand_generate_mt19937(&mt);
```

Once you have it, you can use it to extract a number
either using an integer-based range, like so:
```c
ezra_int_range_t range;
ezra_init_int_range(0, 9, &range); // range [0, 9]
int64_t result = ezra_scale_in_int_range(&range, &rand_num);
```

Or a double, using a real-based range:
```c
ezra_real_range_t range;
ezra_init_open_real_range(0.0, 1.0, &range); // range (0, 1)
double result = ezra_scale_in_real_range(&range, &rand_num);
```