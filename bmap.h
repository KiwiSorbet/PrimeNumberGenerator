#ifndef BMAP_H
#define BMAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum direction {
    LEFT,
    RIGHT
};

struct bmap {
    size_t length; // length of the bitmap in bits
    uint8_t* map; // array storing the bit field
};

struct bmap* bmap_create(size_t length, bool init);
void bmap_print(const struct bmap* bmap);
void bmap_free(struct bmap* bmap);
void bmap_extend(struct bmap* bmap, size_t extend_length, bool init);
bool bmap_get(const struct bmap* bmap, size_t index);
size_t bmap_find_next(const struct bmap* bmap, size_t index, bool value,
                      enum direction dir);
void bmap_set(struct bmap* bmap, size_t index, bool value);
void bmap_set_mul(struct bmap* bmap, size_t index, size_t cnt, bool value);

#endif