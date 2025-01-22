#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

typedef uint64_t prime; // type for storing prime numbers

// Parses command line arguments and edits global variables accordingly before
// prime number generation starts.
void parse_arguments(int argc, char* argv[]);

// Marks the multiples of a prime number through the assoiated bitmap as
// non-primes starting from a specific index.
void progagate_prime(prime prime_num, size_t start_index, struct bmap* bmap);

#endif
