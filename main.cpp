#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "position.h"
#include "board_state.h"

using namespace StopLearn;


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
		{
			auto moves = board.getMoves(player).release();
			if(moves->empty()) break;
			
			ControllerMap* controller = ControllerMap::create(&board).release();
			bool valid = controller->getCount(Controller::None) > 0 || controller->getCount(Controller::OwnerBoth) > 0;
			
			delete controller;
			if(!valid) {
				puts("No move intersections!");
			}
	
			Board bestBoard;
			int bestScore = 0;
	
			for(const auto& move : *moves) {
				Board testBoard = board;
				testBoard.setCell(move, player);
				ControllerMap* testController = ControllerMap::create(&testBoard).release();
				
				int testScore = testController->getCount(Controller::Owner1)  - testController->getCount(Controller::Owner2);
				if(player == Player::P1 ? testScore > bestScore : testScore < bestScore) { // MinMax
					bestScore = testScore;
					bestBoard = testBoard;
				}
				
				delete testController;
			}
			
			board = bestBoard;
			delete moves;
		
			board.print();
		}
		#if 1
		{
			// Check for no moves
			auto moves = board.getMoves(Player::P2).release();
			if(moves->empty()) {
				player = otherPlayer(player);
				break;
			}
			
			bool found = false;
			unsigned int x = 0, y = 0;
			while(!found) {
				fputs("> ", stdout);
				
				int c = getchar();
				while(c == '\n') c = getchar();
				if(c == '?') {
					ControllerMap* controller = ControllerMap::create(&board).release();
					controller->print();
					delete controller;
					fputs("> ", stdout);
				} else {
					ungetc(c, stdin);
				}
	
				while(!scanf("%u,%u", &x, &y) || x >= BOARD_SIZE || y >= BOARD_SIZE) { 
					fputs("Expected 'x,y'\n> ", stdout); 
					x = 0, y = 0;
					while (getchar() != '\n');
				}
				
				printf("Moving to %d, %d\n", x, y);

				for(const auto& move : *moves) {
					if(move.x == x && move.y == y) {
						found = true;
						break;
					}
				}
				
				if(!found) puts("Cannot move here");
			}
			
			delete moves;
			board.setCell(POSITION(x, y), Player::P2);
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
