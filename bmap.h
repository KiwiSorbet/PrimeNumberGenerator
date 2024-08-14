#ifndef BMAP_H
#define BMAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum direction {
    LEFT,
    RIGHT
};

// Struct to store a bitmap object
struct bmap {
    size_t length; // length of the bitmap in bits
    uint8_t* map; // array storing the bit field
};

// Creates and initializes a bitmap of the input length in bits.
struct bmap* bmap_create(size_t length, bool init);

// Prints the contents of the associated bitmap.
void bmap_print(const struct bmap* bmap);

// Frees the associated bitmap from memory.
void bmap_free(struct bmap* bmap);

// Extends the length in bits of the bitmap in a bitmap struct.
void bmap_extend(struct bmap* bmap, size_t extend_length, bool init);

// Returns the value of a certain location in the associated bitmap.
bool bmap_get(const struct bmap* bmap, size_t index);

// Finds the first occurence of a certain value in the bitmap from a starting
// index (including the starting index) by walking through the bitmap in a
// certain direction and returns its associated index. Returns the length of the
// bitmap if no occurence was found.
size_t bmap_find_next(const struct bmap* bmap, size_t index, bool value,
                      enum direction dir);

// Sets a specific location in the associated bitmap to a specific value.
void bmap_set(struct bmap* bmap, size_t index, bool value);

// Sets a number cnt of specific bits from a starting index in the associated
// bitmap to a specific value.
void bmap_set_mul(struct bmap* bmap, size_t index, size_t cnt, bool value);

#endif
