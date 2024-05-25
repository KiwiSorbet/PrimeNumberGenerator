#ifndef BMAP_H
#define BMAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct bmap {
    size_t length; // length of the bitmap in bits
    uint8_t* map; // array storing the bit field
};

enum err_code {
    SUCCESS,
    FAILURE
};

enum direction {
    LEFT,
    RIGHT
};

struct bmap* bmap_create(size_t length, bool init);
void bmap_free(struct bmap* bmap);
enum err_code bmap_extend(struct bmap* bmap, size_t extend_length, bool init);
void bmap_print(struct bmap* bmap);
bool bmap_get(struct bmap* bmap, size_t index);
void bmap_set(struct bmap* bmap, size_t index, bool value);
void bmap_set_mul(struct bmap* bmap, size_t index, size_t cnt, bool value);
size_t bmap_find_next(struct bmap* bmap, size_t index, bool value,
                      enum direction dir);

#endif