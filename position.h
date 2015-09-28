#ifndef __POSITION
#define __POSITION

#include <stdint.h>

typedef struct Position {
    unsigned int x : 4;
    unsigned int y : 4;
    uint8_t size;
    struct Position* next;
} Position;

Position* Position_create(uint8_t x, uint8_t y, Position* next);
void Position_destroy(Position* position);
void Position_print(Position* position);

#endif