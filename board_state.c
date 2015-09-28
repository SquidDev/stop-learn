#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "board.h"
#include "board_state.h"

// Horrible macro.
#define CHECK_PLAYER(player, cell) if(player == PLAYER_NONE) { player = cell; } else if(player != cell) { return CONTROLLER_BOTH; }

uint8_t Board_controller(Board* board, uint8_t x, uint8_t y) {
    // Technically we don't need this.
    uint8_t player = Board_cell(board, x, y);
    if(player != PLAYER_NONE) return player;

	if(y > 0) { // Above
		uint8_t cell = Board_cell(board, x, y - 1);
		if(cell != PLAYER_NONE) {
		    CHECK_PLAYER(player, cell);
		} else if(y > 1) {
		    cell = Board_cell(board, x, y - 2);
		    CHECK_PLAYER(player, cell);
		}
	}

	if(y < NUM_ROWS - 1) { // Below
		uint8_t cell = Board_cell(board, x, y + 1);
		if(cell != PLAYER_NONE) {
		    CHECK_PLAYER(player, cell);
		} else if(y < NUM_ROWS - 2) {
		    cell = Board_cell(board, x, y + 2);
		    CHECK_PLAYER(player, cell);
		}
	}

	if(x > 0) { // Left
		uint8_t cell = Board_cell(board, x - 1, y);
		if(cell != PLAYER_NONE) {
		    CHECK_PLAYER(player, cell);
		} else if(x > 1) {
		    cell = Board_cell(board, x - 2, y);
		    CHECK_PLAYER(player, cell);
		}
	}

	if(x < NUM_COLS - 1) { // Right
		uint8_t cell = Board_cell(board, x + 1, y);
		if(cell != PLAYER_NONE) {
		    CHECK_PLAYER(player, cell);
		} else if(x > NUM_COLS - 2) {
		    cell = Board_cell(board, x + 2, y);
		    CHECK_PLAYER(player, cell);
		}
	}

	return player;
}

BoardControllers* Board_controllers(Board* board) {
    BoardControllers* controllers = malloc(sizeof(board));
    memcpy(controllers, board, sizeof(board));
    
    bool hasNone = true;
    while(hasNone) {
    	hasNone = false;
	    for(uint8_t x = 0; x < NUM_COLS; ++x) {
			for(uint8_t y = 0; y < NUM_ROWS; ++y) {
				uint8_t controller = Board_controller(controllers, x, y);
				if(controller == CONTROLLER_NONE) hasNone = true;
				Board_override(board, x, y, controller);
			}
		}
    }
    
    return controllers;
}
