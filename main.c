#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmap.h"
#include "main.h"

#define DEFAULT_LIST_SIZE 1000000 // how many prime numbers to find

bool write_values = false;
size_t list_size = DEFAULT_LIST_SIZE;

int main(int argc, char* argv[]) {
    // parse command line arguments
    parse_arguments(argc, argv);

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

void parse_arguments(int argc, char* argv[]) {
    // check if there are arguments
    if (argc < 2)
        return;

    // parse individual arguments
    for (size_t i = 1; i < (size_t) argc; i++) {
        // check for -w and --write (writing prime numbers to a file)
        if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--write") == 0) {
            write_values = true;
        }
        // check for --num (number of prime numbers to be generated)
        else if (strcmp(argv[i], "--num") == 0) {
            // check for missing value after argument
            if (i + 1 >= (size_t) argc) {
                printf("Missing value after argument \"--num\"\n");
                exit(-1);
            }

            // check that the value fed is a positive integer
            bool value_is_valid = true;
            for (size_t j = 0; j < strlen(argv[i + 1]); j++) {
                if (argv[i + 1][j] < '0' || argv[i + 1][j] > '9') {
                    printf("Invalid value for argument \"--num\". Value must "
                           "be a positive integer\n");
                    value_is_valid = false;
                    exit(-1);
                }
            }

            // edit the prime list size with the value fed
            if (value_is_valid)
                sscanf(argv[i + 1], "%zu", &list_size);
            i++; // increment i to jump 2 tokens in the argument list
        }
        else {
            printf("Unrecognized argument: \"%s\"\n", argv[i]);
            exit(-1);
        }
    }
}

void progagate_prime(prime prime_num, size_t start_index, struct bmap* bmap) {
    size_t multiplier = start_index / prime_num;
    size_t multiple = prime_num * multiplier;

    while (multiple < bmap->length) {
        bmap_set(bmap, multiple, false);
        multiple += prime_num;
    }
}
