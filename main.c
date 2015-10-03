#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "board.h"
#include "position.h"
#include "board_state.h"


int main(void)
{
	// http://stanford.edu/~loetting/MachineLearning.pdf
	// https://en.wikipedia.org/wiki/Reinforcement_learning
	// https://en.wikipedia.org/wiki/Markov_decision_process
	srand((unsigned int)time(NULL));
	
	// Board* board = Board_new();
	// for(int i = 0; i < 8; i++) Board_move(board, i, 3, PLAYER_2);
	
	// Board_print(board);
	
	// BoardControllers* controller = BoardControllers_new(board);
	// BoardControllers_print(controller);
	
	
	// free(controller);
	// free(board);
	
	Board* board = Board_new();
	Board* testBoard = malloc(sizeof(Board));
	Board* bestBoard = malloc(sizeof(Board));

	uint8_t player = PLAYER_1;

	Board_print(board);
	while(1) {
		Position* position = Board_moves(board, player);
		if(position == NULL) break;

		printf("Player %d has %d moves\n", player - 1, position->size);
		Position* target = position;
		
		BoardControllers* bestController = NULL;
		int bestScore = 0;

		for(int i = 0; i < position->size; i++) {
			memcpy(testBoard, board, sizeof(Board));

			Board_move(testBoard, target->x, target->y, player);
			BoardControllers* testController = BoardControllers_new(testBoard);
			
			int testScore = testController->counts[CTRL_OWNER_1]  - testController->counts[CTRL_OWNER_2];
			if(bestController == NULL) {
				bestScore = testScore;
				bestController = testController;
				memcpy(bestBoard, testBoard, sizeof(Board));
			} else if(player == PLAYER_1 ? testScore > bestScore : testScore < bestScore) { // MinMax
				free(bestController);
				bestScore = testScore;
				bestController = testController;
				memcpy(bestBoard, testBoard, sizeof(Board));
			} else {
				free(testController);
			}
			target = target->next;
		}

		// We've moved
		memcpy(board, bestBoard, sizeof(Board));
		// BoardControllers_print(bestController);
		free(bestController);
		Position_destroy(position);
	
		Board_print(board);
		#if 1
		// Check for no moves
		position = Board_moves(board, PLAYER_2);
		if(position == NULL) {
			player = Player_other(player);
			break;
		}
		
		int found = 0;
		uint8_t x = 0, y = 0;
		while(!found) {
			unsigned int xScan = 0, yScan = 0;
			fputs("> ", stdout);
			
			int c = getchar();
			while(c == '\n') c = getchar();
			if(c == '?') {
				puts("Read ?");
				BoardControllers* controller = BoardControllers_new(board);
				BoardControllers_print(controller);
				free(controller);
				fputs("> ", stdout);
			} else {
				ungetc(c, stdin);
			}
			while(!scanf("%u,%u", &xScan, &yScan)) { 
				fputs("Expected 'x,y'\n> ", stdout); 
				xScan = 0, yScan = 0;
				while (getchar() != '\n');
			}
			
			x = (uint8_t) xScan, y = (uint8_t) yScan;
			printf("Moving to %d, %d\n", x, y);

			target = position;
			for(int i = position->size; i > 0; i--) {
				if(target->x == x && target->y == y) {
					found = 1;
					break;
				}
				target = target->next;
			}
			
			if(!found) puts("Cannot move here");
		}
		
		Position_destroy(position);
		Board_move(board, x, y, PLAYER_2);
		#else
			while(getchar() == '?') {
				BoardControllers* controller = BoardControllers_new(board);
				BoardControllers_print(controller);
				free(controller);
			}
			player = Player_other(player);
		#endif
	}

	Board_print(board);
	
	free(board);
	free(testBoard);
	free(bestBoard);
	printf("Player %d wins!\n", Player_other(player) - 1);

	return 0;
}
