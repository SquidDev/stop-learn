#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "position.h"
#include "moving.h"
#include "scoring/controller.h"
#include "scoring/floodFill.h"

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

	board.print();
	while(1) {
		if(!bestMove(&FloodFill::score, board, player, board)) {
			break;
		}
		board.print();
		#if 1
		{
			if(!userMove(board, Player::P2, board)) {
				player = Player::P2;
				break;
			}
		}
		#else
			while(getchar() == '?') {
				ControllerMap* controller = ControllerMap::create(&board).release();
				controller->print();
				delete controller;
			}
			player = otherPlayer(player);
		#endif
	}

	board.print();
	printf("Player %d wins!\n", static_cast<uint8_t>(otherPlayer(player)) - 1);

	return 0;
}
