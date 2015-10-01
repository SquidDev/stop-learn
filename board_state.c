#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "board.h"
#include "board_state.h"
#include "position.h"

uint8_t BoardControllers_cell(BoardControllers* controllers, uint8_t x, uint8_t y) { return (controllers->rows[y] >> (x * CTRL_WIDTH)) & CTRL_FLAG; }

// Horrible macro.
#define CHECK_PLAYER(player, cell) if(cell != PLAYER_NONE) { if(player == PLAYER_NONE) { player = cell; } else if(player != cell) { return CTRL_OWNER_BOTH; } }

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

	if(y < BOARD_SIZE - 1) { // Below
		uint8_t cell = Board_cell(board, x, y + 1);
		if(cell != PLAYER_NONE) {
		    CHECK_PLAYER(player, cell);
		} else if(y < BOARD_SIZE - 2) {
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

	if(x < BOARD_SIZE - 1) { // Right
		uint8_t cell = Board_cell(board, x + 1, y);
		if(cell != PLAYER_NONE) {
		    CHECK_PLAYER(player, cell);
		} else if(x > BOARD_SIZE - 2) {
		    cell = Board_cell(board, x + 2, y);
		    CHECK_PLAYER(player, cell);
		}
	}

	return player == CTRL_NONE ? CTRL_NONE : player + 2;
}
#undef CHECK_PLAYER

bool BoardControllers_floodFill(BoardControllers* from, uint8_t ownerValue, uint8_t x, uint8_t y) {
	Position* stack = Position_create(x, y, NULL);
	
	if(x > 0) stack = Position_create(x - 1, y, stack);
	if(x < BOARD_SIZE - 1) stack = Position_create(x + 1, y, stack);
	if(y > 0) stack = Position_create(x, y - 1, stack);
	if(y < BOARD_SIZE - 1) stack = Position_create(x, y + 1, stack);
	
	while(stack != NULL) {
		Position* current = stack;
		uint8_t x = current->x, y = current->y;
		
		// Pop stack
		stack = current->next;
		free(current);

		uint8_t value = BoardControllers_cell(from, x, y);
		if(value == CTRL_NONE) {
			from->rows[y] |= ownerValue << (x * CTRL_WIDTH);
			
			// If we've overridden this value then scan for others
			if(x > 0) stack = Position_create(x - 1, y, stack);
			if(x < BOARD_SIZE - 1) stack = Position_create(x + 1, y, stack);
			if(y > 0) stack = Position_create(x, y - 1, stack);
			if(y < BOARD_SIZE - 1) stack = Position_create(x, y + 1, stack);
		} else if(value != ownerValue && value != ownerValue - 2) {
			// We've hit an illegal value: Abort
			Position_destroy(stack);
			return false;
		}
	}
	
	return true;
}

BoardControllers* BoardControllers_new(Board* board) {
    BoardControllers* controllers = calloc(1, sizeof(BoardControllers));
    BoardControllers* tempController = malloc(sizeof(BoardControllers));
    
    Position* stack = NULL;
    for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
    	for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
			uint8_t controller = Board_controller(board, x, y);
			if(controller == CTRL_OWNER_1 || controller == CTRL_OWNER_2) {
				stack = Position_create(x, y, stack);
				stack->size = controller; // Abuse size property
			}

			controllers->rows[y] |= controller << (x * CTRL_WIDTH);
			controllers->counts[controller]++;
		}
	}
	
	memcpy(tempController, controllers, sizeof(BoardControllers));
	while(stack != NULL) {
		if(BoardControllers_floodFill(tempController, stack->size, stack->x, stack->y)) {
			memcpy(controllers, tempController, sizeof(BoardControllers));
		} else {
			memcpy(tempController, controllers, sizeof(BoardControllers));
		}
		
		Position* current = stack;
		stack = stack->next;
		free(current);
	}
	
	free(tempController);
    
    return controllers;
}

char BoardControllers_char(uint8_t c) {
	switch(c) {
		case CTRL_NONE:
			return ' ';
		case CTRL_OCCUPIED_1:
			return 'X';
		case CTRL_OCCUPIED_2:
			return 'O';
		case CTRL_OWNER_1:
			return 'x';
		case CTRL_OWNER_2:
			return 'o';
		case CTRL_OWNER_BOTH:
			return '?';
		default:
			return '.';
	}
}

void BoardControllers_print(BoardControllers* controllers) {
	puts("  0 1 2 3 4 5 6 7");
	for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
		printf("%d|", y);
		for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
			putchar(BoardControllers_char(BoardControllers_cell(controllers, x, y)));
			putchar('|');
		}

		putchar('\n');
	}
	
	for(uint8_t i = 0; i < 6; i++) {
		printf("'%c': %d, ", BoardControllers_char(i), controllers->counts[i]);
	}
	putchar('\n');
}