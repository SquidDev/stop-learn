#ifndef __BOARD_STATE
#define __BOARD_STATE

#include <stdint.h>
#include "board.h"

// The maximum value is 5, which is 3 bits long
#define CTRL_WIDTH 4
// We could define the flag as 15, but we don't need the spare bit
#define CTRL_FLAG 7

// No-one owns it
#define CTRL_NONE 0

// A place has been played here
#define CTRL_OCCUPIED_1 2
#define CTRL_OCCUPIED_2 3
// A player technically can play here next move or surrounds it
#define CTRL_OWNER_1 4
#define CTRL_OWNER_2 5
#define CTRL_OWNER_BOTH 1

typedef struct BoardControllers {
    uint32_t rows[BOARD_SIZE];
    uint8_t counts[6];
} BoardControllers;

uint8_t Board_controller(Board* board, uint8_t x, uint8_t y);
BoardControllers* BoardControllers_new(Board* board);
void BoardControllers_print(BoardControllers* controllers);
bool BoardControllers_floodFill(BoardControllers* from, uint8_t ownerValue, uint8_t x, uint8_t y);
char BoardControllers_char(uint8_t c);

uint8_t BoardControllers_cell(BoardControllers* controllers, uint8_t x, uint8_t y);
#endif
