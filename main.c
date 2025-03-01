#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmap.h"
#include "main.h"

#define DEFAULT_LIST_SIZE 1000000 // Default size of prime number list

bool write_values = false; // Whether to write the primes to a file or not
size_t list_size = DEFAULT_LIST_SIZE; // Size of prime number list

int main(int argc, char* argv[]) {
    // Parse and interpret command line arguments
    parse_arguments(argc, argv);

    // Create bitmap
    size_t bmap_increment = list_size; // Size of chunks of bitmap allocated
    struct bmap* prime_map = bmap_create(bmap_increment, true);
    if (prime_map == NULL)
        return -1;

    // Initialize bitmap
    bmap_set(prime_map, 0, false); // 0 is not prime
    bmap_set(prime_map, 1, false); // 1 is not prime
    size_t bmap_index = 1;

    // Create the list of prime numbers
    prime* prime_list = malloc(list_size * sizeof(prime));
    if (prime_list == NULL)
        return -1;
    size_t list_index = 0;

    // Find prime numbers until list is filled
    for (; list_index < list_size; list_index++) {
        // Find the next prime number
        bmap_index++;
        bmap_index = bmap_find_next(prime_map, bmap_index, true, RIGHT);

        // If end of bitmap reached, need to extend it
        if (bmap_index == prime_map->length) {
            // Extend bitmap
            size_t old_length = prime_map->length; // Save old length of bitmap
            bmap_extend(prime_map, bmap_increment, true);

            // Update new bitmap entries with previous primes
            for (size_t i = 0; i < list_index; i++)
                progagate_prime(prime_map, prime_list[i], old_length);

            // Recalculate bitmap index of the next new prime
            bmap_index = bmap_find_next(prime_map, bmap_index, true, RIGHT);
        }

        // Update bitmap and prime list
        prime_list[list_index] = bmap_index;
        progagate_prime(prime_map, bmap_index, bmap_index);
    }

    // Print last prime number in the list
    printf("%zuth prime number: %zu\n", list_index, prime_list[list_size - 1]);

    // Write all prime numbers to a file
    if (write_values) {
        // Create file
        FILE* file = fopen("primes.txt", "w");
        if (file == NULL) {
            perror("Error opening file");
            exit(-1);
        }

        // Write values to file
        fprintf(file, "POSITION\t\tVALUE\n");
        for (size_t i = 0; i < list_size; i++)
            fprintf(file, "%zu\t\t\t\t%zu\n", i + 1, prime_list[i]);

        fclose(file);
    }

    // Deallocate memory before exiting
    free(prime_list);
    bmap_free(prime_map);
    return 0;
}

void progagate_prime(struct bmap bmap[], prime prime_num, size_t start_index) {
    size_t multiplier = start_index / prime_num;
    size_t multiple = prime_num * multiplier;

    while (multiple < bmap->length) {
        bmap_set(bmap, multiple, false);
        multiple += prime_num;
    }
}

void parse_arguments(int argc, char* argv[]) {
    // Check if there are arguments
    if (argc < 2)
        return;

    // Parse individual arguments
    for (size_t i = 1; i < (size_t) argc; i++) {
        // Check for --write (writing prime numbers to a file)
        if (strcmp(argv[i], "--write") == 0) {
            write_values = true;
        }
        // Check for --num (number of prime numbers to be generated)
        else if (strcmp(argv[i], "--num") == 0) {
            // Check for missing value after argument
            if (i + 1 >= (size_t) argc) {
                printf("Missing value after argument \"--num\".\n");
                exit(-1);
            }

            // Check that the value fed is a positive integer
            bool value_is_valid = true;
            for (size_t j = 0; j < strlen(argv[i + 1]); j++) {
                if (argv[i + 1][j] < '0' || argv[i + 1][j] > '9') {
                    printf("Invalid value for argument \"--num\". Value must "
                           "be a positive integer.\n");
                    value_is_valid = false;
                    exit(-1);
                }
            }

            // Edit the prime list size with the value fed
            if (value_is_valid)
                sscanf(argv[i + 1], "%zu", &list_size);
            i++; // Increment i to jump 2 tokens in the argument list
        }
        else {
            printf("Unrecognized argument: \"%s\"\n", argv[i]);
            exit(-1);
        }
    }
}
