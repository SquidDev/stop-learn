#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "position.h"
#include "moving.h"
#include "scoring/controller.h"
#include "scoring/floodFill.h"
#include "scoring/manhattan.h"

using namespace StopLearn;
using namespace StopLearn::Scoring;
using namespace StopLearn::Moving;

int main(void)
{
	// http://stanford.edu/~loetting/MachineLearning.pdf
	// https://en.wikipedia.org/wiki/Reinforcement_learning
	// https://en.wikipedia.org/wiki/Markov_decision_process

	// Or maybe:
	// https://en.wikipedia.org/wiki/Minimax#Pseudocode
	// https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning#Pseudocode
	
	Board board;
	board.setup();
	Player player = Player::P1;
	
	BoardMover playerOne = minmax(&ManhattanMap::score, 2);
	#if 1
		BoardMover playerTwo = &userMove;
	#else
		BoardMover playerTwo = bestMove(&FloodFill::score);
	#endif

	while(1) {
		if(!board.canMove(player)) break;
		
		board = playerOne(board, player);
		board.print();
		
		player = otherPlayer(player);
		if(!board.canMove(player)) break;
		board = playerTwo(board, player);
		
		player = otherPlayer(player);
	}

	board.print();
	printf("Player %d wins!\n", static_cast<uint8_t>(otherPlayer(player)) - 1);

	return 0;
}
