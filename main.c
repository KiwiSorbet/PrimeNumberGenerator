#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmap.h"
#include "main.h"

#define DEFAULT_LIST_SIZE 1000000 // how many prime numbers to find

int main(int argc, char* argv[]) {
    // get list size from command line arguments
    size_t list_size;
    if (argc == 2)
        sscanf(argv[1], "%zu", &list_size);
    else
        list_size = DEFAULT_LIST_SIZE;

    // create bitmap
    struct bmap* prime_map = bmap_create(list_size, true);
    if (prime_map == NULL)
        return -1;

    // initialize bitmap
    bmap_set(prime_map, 0, false); // 0 is not prime
    bmap_set(prime_map, 1, false); // 1 is not prime
    size_t bmap_index = 1;

    // create the list of prime numbers
    prime* prime_list = malloc(list_size * sizeof(prime));
    if (prime_list == NULL)
        return -1;
    size_t list_index = 0;

    // find prime numbers until list is filled
    for (; list_index < list_size; list_index++) {
        // find the next prime number
        bmap_index = bmap_find_next(prime_map, ++bmap_index, true, RIGHT);

        // if end of bitmap reached, need to extend it
        if (bmap_index == prime_map->length) {
            // extend bitmap
            size_t old_length = prime_map->length; // save old length of bitmap
            bmap_extend(prime_map, list_size, true);

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
    printf("%zuth prime number: %zu\n", list_index, prime_list[list_size - 1]);
    free(prime_list);
    bmap_free(prime_map);
}

void progagate_prime(prime prime_num, size_t start_index, struct bmap* bmap) {
    size_t multiplier = start_index / prime_num;
    size_t multiple = prime_num * multiplier;

    while (multiple < bmap->length) {
        bmap_set(bmap, multiple, false);
        multiple += prime_num;
    }
}
