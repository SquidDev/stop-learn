#ifndef __BOARD_STATE
#define __BOARD_STATE

#include <stdint.h>
#include "board.h"

// Both players control it
#define CONTROLLER_BOTH PLAYER_NONE

// Neither player controls
#define CONTROLLER_NONE 1

typedef struct BoardState {
    uint8_t unowned;
    uint8_t player1;
    uint8_t player2;
} BoardState;

// A type alias as they are structually similar
typedef Board BoardControllers;

uint8_t Board_controller(Board* board, uint8_t x, uint8_t y);
BoardControllers* Board_controllers(Board* board);
BoardState* Board_getState(BoardControllers* board);
#endif