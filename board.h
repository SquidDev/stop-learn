#ifndef __BOARD
#define __BOARD

#include <stdint.h>
#include <stdbool.h>
#include "position.h"

// You can swap between using ^ 1
#define PLAYER_NONE 0
#define PLAYER_1 2
#define PLAYER_2 3

#define NUM_ROWS 8
#define NUM_COLS 8
#define COL_FLAG 3

typedef struct Board {
    uint16_t rows[NUM_ROWS];
} Board;

uint8_t Player_other(uint8_t player);

uint8_t Board_cell(Board* board, uint8_t x, uint8_t y);
uint8_t Board_column(uint16_t row, uint8_t column);

Board* Board_new(void);
Position* Board_moves(Board* board, uint8_t player);
bool Board_anyMove(Board* board, uint8_t player);
bool Board_canMove(Board* board, uint8_t player, uint8_t x, uint8_t y);
void Board_move(Board* board, uint8_t x, uint8_t y, uint8_t player);
void Board_override(Board* board, uint8_t x, uint8_t y, uint8_t player);
void Board_print(Board* board);

#endif
