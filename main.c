#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include "board.h"
#include "position.h"
#include "board_state.h"

// Between 0 and limit exclusive
int rand_lim(int limit) {
    int divisor = RAND_MAX/limit;
    int retval;

    do {
        retval = rand() / divisor;
    } while (retval > limit - 1);

    return retval;
}

int main(void)
{
	// http://stanford.edu/~loetting/MachineLearning.pdf
	// https://en.wikipedia.org/wiki/Reinforcement_learning
	// https://en.wikipedia.org/wiki/Markov_decision_process
	srand(time(NULL));
	
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
		int8_t bestScore = 0;

		for(int i = 0; i < position->size; i++) {
			memcpy(testBoard, board, sizeof(Board));

			Board_move(testBoard, target->x, target->y, player);
			BoardControllers* testController = BoardControllers_new(testBoard);
			
			int8_t testScore = testController->counts[CTRL_OWNER_1]  - testController->counts[CTRL_OWNER_2];
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
		free(bestController);
		Position_destroy(position);
		
		// Board_print(board);
		// getchar();

		player = Player_other(player);
	}

	Board_print(board);
	
	free(board);
	free(testBoard);
	free(bestBoard);
	printf("Player %d wins!\n", Player_other(player) - 1);

	return 0;
}