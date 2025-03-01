#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmap.h"

#define bmap_int uint64_t
#define int_size sizeof(bmap_int)

struct bmap* bmap_create(size_t length, bool init) {
    // Allocate memory for bitmap struct
    struct bmap* bmap = malloc(sizeof(struct bmap));
    if (bmap == NULL)
        return NULL;

    // Get the length in bytes to allocate (rounded up to the nearest byte)
    size_t byte_length = ((length + 7) / 8);
    bmap->length = byte_length * 8;

    // Allocate memory for bitmap
    bmap->map = malloc(byte_length);
    if (bmap->map == NULL) {
        free(bmap);
        return NULL;
    }

    // Set all bits to initial value
    memset(bmap->map, (init == true) ? 0b11111111 : 0b00000000, byte_length);

    return bmap;
}

void bmap_print(const struct bmap bmap[]) {
    size_t byte_length = (bmap->length + 7) / 8;

    printf("------------------------\n");

    // Print each byte
    for (size_t i = 0; i < byte_length; i++) {
        printf("%6zu | ", i * 8);

        // Print each bit in the byte
        for (size_t j = 0; j < 8; j++)
            printf("%d ", bmap_get(bmap, i * 8 + j));

        printf("\n");
    }

    printf("------------------------\n");
    return;
}

void bmap_free(struct bmap bmap[]) {
    free(bmap->map);
    free(bmap);
}

void bmap_extend(struct bmap bmap[], size_t extend_length, bool init) {
    // Create the new larger bitmap
    size_t new_byte_length = bmap->length / 8 + (extend_length + 7) / 8;
    bmap->map = (uint8_t*) realloc(bmap->map, new_byte_length);
    if (bmap->map == NULL)
        exit(-1);

    // Update bitmap length
    size_t old_length = bmap->length;
    bmap->length = new_byte_length * 8;

    // Set all newly allocated bits to chose
    memset(bmap->map + old_length / 8, (init) ? 0b11111111 : 0b00000000,
           new_byte_length - old_length / 8);
}

bool bmap_get(const struct bmap bmap[], size_t index) {
    // Index out of bounds always returns false
    if (index >= bmap->length)
        return false;

    // Get the index of the corresponding byte and the bit's offset
    size_t byte_index = index / 8;
    size_t offset = index % 8;

    // Extract the relevant bit from the map
    uint8_t binary_control = 0b10000000 >> offset;
    binary_control &= bmap->map[byte_index];

    return (binary_control == 0) ? false : true;
}

size_t bmap_find_next(const struct bmap bmap[], size_t index, bool value,
                      enum direction dir) {
    size_t cursor = index;

    // Walk right
    if (dir == RIGHT)
        while (cursor < bmap->length && bmap_get(bmap, cursor) != value)
            cursor++;
    // Walk left
    else if (dir == LEFT)
        while (bmap_get(bmap, cursor) != value)
            cursor--;

    // Didn't find any occurence
    if (cursor >= bmap->length)
        return bmap->length;

    return cursor;
}

void bmap_set(struct bmap bmap[], size_t index, bool value) {
    // Index out of bounds
    if (index >= bmap->length)
        return;

    // Get the index of the corresponding byte and the bit's offset
    size_t byte_index = index / 8;
    size_t offset = index % 8;

    // Set to true
    if (value == true) {
        uint8_t binary_control = 0b10000000 >> offset;
        bmap->map[byte_index] |= binary_control;
    }
    // Set to false
    else {
        uint8_t binary_control = ~(0b10000000 >> offset);
        bmap->map[byte_index] &= binary_control;
    }
}

void bmap_set_mul(struct bmap bmap[], size_t index, size_t cnt, bool value) {
    for (size_t i = index; i < index + cnt; i++)
        bmap_set(bmap, i, value);
}
