#include <stdlib.h>
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

	Board* board = Board_new();
	uint8_t player = PLAYER_1;

	Board_print(board);
	while(1) {
		Position* position = Board_moves(board, player);
		if(position == NULL) break;

		printf("Player %d has %d moves\n", player - 1, position->size);

		Position* target = position;
		for(int i = rand_lim(position->size); i > 0; i--) {
			target = target->next;
		}

		Board_move(board, target->x, target->y, player);
		Position_destroy(position);
		
		BoardControllers* controllers = Board_controllers(board);
		printf("Controllers:\n");
		Board_print(controllers);
		free(controllers);

		player = Player_other(player);

		// position = Board_moves(board, PLAYER_2);
		// if(position == NULL) {
		// 	player = Player_other(player);
		// 	break;
		// }

		// Board_print(board);
		// int x, y;
		// while(!scanf("%d,%d", &x, &y)) { printf("Invalid: Try again\n"); }
		// printf("%d, %d\n", x, y);
		// int found = 0;
		// target = position;
		// for(int i = position->size; i > 0; i--) {
		// 	if(target->x == x && target->y == y) {
		// 		found = 1;
		// 		break;
		// 	}
		// 	target = target->next;
		// }
		// Position_destroy(position);

		// if(found) {
		// 	Board_move(board, x, y, PLAYER_2);
		// } else {
		// 	printf("Invalid move, skipping\n");
		// }
	}

	Board_print(board);
	free(board);
	printf("Player %d wins!\n", Player_other(player) - 1);

	return 0;
}