#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmap.h"

// Creates and initializes a bitmap of the input length in bits.
struct bmap* bmap_create(size_t length, bool init) {
    // allocate memory for bitmap struct
    struct bmap* bmap = malloc(sizeof(struct bmap));
    if (bmap == NULL)
        return NULL;

    // get the length in bytes to allocate (rounded up to the nearest byte)
    size_t byte_length = ((length + 7) / 8);
    bmap->length = byte_length * 8;

    // allocate memory for bitmap
    bmap->map = malloc(byte_length);
    if (bmap->map == NULL) {
        free(bmap);
        return NULL;
    }

    // set all bits to initial value
    memset(bmap->map, (init == true) ? 0b11111111 : 0b00000000, byte_length);

    return bmap;
}

// Prints the contents of the associated bitmap.
void bmap_print(struct bmap* bmap) {
    size_t byte_length = (bmap->length + 7) / 8;
    size_t cur_bit = 0;

    printf("------------------------\n");

    // print each byte
    for (size_t i = 0; i < byte_length; i++) {
        printf("%6zu | ", i * 8);

        // print each bit in the byte
        for (size_t j = 0; j < 8; j++)
            printf("%d ", bmap_get(bmap, cur_bit++));

        printf("\n");
    }

    printf("------------------------\n");
    return;
}

// Frees the associated bitmap from memory.
void bmap_free(struct bmap* bmap) {
    free(bmap->map);
    free(bmap);
    return;
}

// Extends the length in bits of the bitmap in a bitmap struct.
void bmap_extend(struct bmap* bmap, size_t extend_length, bool init) {
    // create the new larger bitmap
    size_t new_byte_length = bmap->length / 8 + (extend_length + 7) / 8;
    uint8_t* new_map = malloc(new_byte_length);
    if (new_map == NULL) {
        perror("Failed to extend bitmap");
        exit(-1);
    }

    // copy contents of the old bitmap into the new one
    memset(new_map + bmap->length / 8, (init) ? 0b11111111 : 0b00000000,
           new_byte_length - bmap->length / 8);
    memcpy(new_map, bmap->map, bmap->length / 8);

    // assign new bitmap to the bitmap struct
    free(bmap->map);
    bmap->map = new_map;
    bmap->length = new_byte_length * 8;

    return 0;
}

// Returns the value of a certain location in the associated bitmap.
bool bmap_get(struct bmap* bmap, size_t index) {
    // index out of bounds always returns false
    if (index >= bmap->length)
        return false;

    // get the index of the corresponding byte and the bit's offset
    size_t byte_index = index / 8;
    size_t offset = index % 8;

    // extract the relevant bit from the map
    uint8_t binary_control = 0b10000000 >> offset;
    binary_control &= bmap->map[byte_index];

    return (binary_control == 0) ? false : true;
}

// Finds the first occurence of a certain value in the bitmap from a starting
// index (including the starting index) by walking through the bitmap in a
// certain direction and returns its associated index. Returns the length of the
// bitmap if no occurence was found.
size_t bmap_find_next(struct bmap* bmap, size_t index, bool value,
                      enum direction dir) {
    size_t cursor = index;

    // walk right
    if (dir == RIGHT)
        while (cursor < bmap->length && bmap_get(bmap, cursor) != value)
            cursor++;
    // walk left
    else if (dir == LEFT)
        while (bmap_get(bmap, cursor) != value)
            cursor--;

    // didn't find any occurence
    if (cursor >= bmap->length)
        return bmap->length;

    return cursor;
}

// Sets a specific location in the associated bitmap to a specific value.
void bmap_set(struct bmap* bmap, size_t index, bool value) {
    // index out of bounds
    if (index >= bmap->length)
        return;

    // get the index of the corresponding byte and the bit's offset
    size_t byte_index = index / 8;
    size_t offset = index % 8;

    // set to true
    if (value == true) {
        uint8_t binary_control = 0b10000000 >> offset;
        bmap->map[byte_index] |= binary_control;
    }
    // set to false
    else {
        uint8_t binary_control = ~(0b10000000 >> offset);
        bmap->map[byte_index] &= binary_control;
    }

    return;
}

// Sets a number cnt of specific bits from a starting index in the associated
// bitmap to a specific value.
void bmap_set_mul(struct bmap* bmap, size_t index, size_t cnt, bool value) {
    for (size_t i = index; i < index + cnt; i++)
        bmap_set(bmap, i, value);
    return;
}