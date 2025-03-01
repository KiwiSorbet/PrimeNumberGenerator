#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

typedef uint64_t prime; // Type for storing prime numbers

// Marks the multiples of a prime number through the assoiated bitmap as
// non-primes starting from a specific index.
void progagate_prime(struct bmap bmap[], prime prime_num, size_t start_index);

// Parses and interprets command line arguments. Edits global variables
// accordingly before prime number generation starts.
void parse_arguments(int argc, char* argv[]);

#endif
