#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "board.h"
#include "position.h"

Board* Board_new() {
	Board* board = calloc(1, sizeof(Board));
	board->rows[0] = PLAYER_1 << ((BOARD_SIZE - 1) * 2);
	board->rows[BOARD_SIZE - 1] = PLAYER_2;
	return board;
}

// Utility functions - should be inlined
uint8_t Player_other(uint8_t player) { return 1 ^ player; }
uint8_t Board_column(uint16_t row, uint8_t column) { return (row >> (column * 2)) & COL_FLAG; }
uint8_t Board_cell(Board* board, uint8_t x, uint8_t y) { return Board_column(board->rows[y], x); }

void Board_print(Board* board) {
	printf("  0 1 2 3 4 5 6 7\n");
	for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
		printf("%d|", y);
		for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
			switch(Board_cell(board, x, y)) {
				default:
					printf(" ");
					break;
				case PLAYER_1:
					printf("X");
					break;
				case PLAYER_2:
					printf("0");
					break;
				case 1:
					printf("?");
					break;
			}
			printf("|");
		}

		printf("\n");
	}
}

Position* Board_moves(Board* board, uint8_t player) {
	Position* position = NULL;

	for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
		for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
			if(Board_canMove(board, player, x, y)) {
				position = Position_create(x, y, position);
				continue;
			}
		}
	}

	return position;
}

bool Board_anyMove(Board* board, uint8_t player) {
	for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
		for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
			if(Board_canMove(board, player, x, y)) {
				return true;
			}
		}
	}
	
	return false;
}

bool Board_canMove(Board* board, uint8_t player, uint8_t x, uint8_t y) {
	// Only search in empty cells
	if(Board_cell(board, x, y) != PLAYER_NONE) return false;

	if(y > 0) { // Above
		uint8_t cell = Board_cell(board, x, y - 1);
		if(
			(cell == player) ||
			(cell == PLAYER_NONE && y > 1 && Board_cell(board, x, y - 2) == player) // Empty above and player two above
		) {
			return true;
		}
	}

	if(y < BOARD_SIZE - 1) { // Below
		uint8_t cell = Board_cell(board, x, y + 1);
		if(
			(cell == player) ||
			(cell == PLAYER_NONE && y < BOARD_SIZE - 2 && Board_cell(board, x, y + 2) == player)
		) {
			return true;
		}
	}

	if(x > 0) { // Left
		uint8_t cell = Board_cell(board, x - 1, y);
		if(
			(cell == player) ||
			(cell == PLAYER_NONE && x > 1 && Board_cell(board, x - 2, y) == player)
		) {
			return true;
		}
	}

	if(x < BOARD_SIZE - 1) { // Right
		uint8_t cell = Board_cell(board, x + 1, y);
		if(
			(cell == player) ||
			(cell == PLAYER_NONE && x < BOARD_SIZE - 2 && Board_cell(board, x + 2, y) == player)
		) {
			return true;
		}
	}

	return false;
}

void Board_move(Board* board, uint8_t x, uint8_t y, uint8_t player) {
	// Doesn't support overriding players. Meh.
	board->rows[y] |= player << (x * 2);
}
void Board_override(Board* board, uint8_t x, uint8_t y, uint8_t player) {
	board->rows[y] &= (~(COL_FLAG << (x * 2))) | (player << (x * 2));
}
