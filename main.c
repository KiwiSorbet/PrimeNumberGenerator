#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmap.h"

#define BMAP_INCREMENT 10000000 // bitmap is incrementally extended
#define LIST_SIZE 1000000 // how many prime numbers to find

typedef uint64_t prime;

void progagate_prime(prime prime_num, size_t start_index, struct bmap* bmap);

int main() {
    // create bitmap
    struct bmap* prime_map = bmap_create(BMAP_INCREMENT, true);
    if (prime_map == NULL)
        return -1;

    // initialize bitmap
    bmap_set(prime_map, 0, false); // 0 is not prime
    bmap_set(prime_map, 1, false); // 1 is not prime
    size_t bmap_index = 1;

    // create the list of prime numbers
    prime* prime_list = malloc(LIST_SIZE * sizeof(prime));
    if (prime_list == NULL)
        return -1;
    size_t list_index = 0;

    // find prime numbers until list is filled
    for (; list_index < LIST_SIZE; list_index++) {
        // find the next prime number
        bmap_index = bmap_find_next(prime_map, ++bmap_index, true, RIGHT);

        // if end of bitmap reached, need to extend it
        if (bmap_index == prime_map->length) {
            // extend bitmap
            size_t old_length = prime_map->length; // save old length of bitmap
            enum err_code err = bmap_extend(prime_map, BMAP_INCREMENT, true);
            if (err == FAILURE)
                return -1;

            // update new bitmap entries with previous primes
            for (size_t i = 0; i < list_index; i++)
                progagate_prime(prime_list[i], old_length, prime_map);

            // recalculate bitmap index of the next new prime
            bmap_index = bmap_find_next(prime_map, bmap_index, true, RIGHT);
        }

        // update bitmap and prime list
        prime_list[list_index] = bmap_index;
        progagate_prime(bmap_index, bmap_index, prime_map);
    }

    // print last prime number in the list
    printf("%zuth prime number: %zu\n", list_index, prime_list[LIST_SIZE - 1]);
    free(prime_list);
    bmap_free(prime_map);

    return 0;
}

// Marks the multiples of a prime number through the assoiated bitmap as
// non-primes starting from a specific index.
void progagate_prime(prime prime_num, size_t start_index, struct bmap* bmap) {
    size_t multiplier = start_index / prime_num;
    size_t multiple = prime_num * multiplier;

    while (multiple < bmap->length) {
        bmap_set(bmap, multiple, false);
        multiple += prime_num;
    }

    return;
}