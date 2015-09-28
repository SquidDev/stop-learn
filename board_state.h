#ifndef __BOARD_STATE
#define __BOARD_STATE

#include "board.h"

typedef struct BoardState {
    uint8_t unowned;
    uint8_t player1;
    uint8_t player2;
} BoardState;

BoardState* Board_getState(Board* board);
#endif