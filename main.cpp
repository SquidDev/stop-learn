#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
using namespace std;

int main(void)
{
	srand(unsigned(time(0)));
	
	// http://stanford.edu/~loetting/MachineLearning.pdf
	// https://en.wikipedia.org/wiki/Reinforcement_learning
	// https://en.wikipedia.org/wiki/Markov_decision_process
	// https://www.cs.bris.ac.uk/Publications/Papers/2000100.pdf
	
	Board board;
	board.setup();
	Player player = Player::P1;
	
	BoardMover playerOne = minmax(&ManhattanMap::score, 2);
	#if 1
		BoardMover playerTwo = &userMove;
	#else
		BoardMover playerTwo = minmax(&FloodFill::score, 2);
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
