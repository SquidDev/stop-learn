#include "moving.h"

#include <stdint.h>
#include <stdio.h>
#include <forward_list>
#include "scoring/floodFill.h"

using namespace std;
using namespace StopLearn::Scoring;

namespace StopLearn { namespace Moving {
    bool bestMove(const Scoring::BoardScorer scorer, const Board& board, const Player player, Board& result) {
        unique_ptr<forward_list<Position>> moves = board.getMoves(player);
        if(moves->empty()) return false;
        
        int8_t bestScore = 0;
        for(const auto& move : *moves) {
			Board testBoard = board;
			testBoard.setCell(move, player);
			int8_t testScore = scorer(&board);
			
			if(player == Player::P1 ? testScore > bestScore : testScore < bestScore) { // MinMax
				bestScore = testScore;
				result = testBoard;
			}
		}
		
		return true;
    }
    
    bool userMove(const Board& board, const Player player, Board& result) {
        // Check for no moves
		unique_ptr<forward_list<Position>> moves = board.getMoves(player);
		if(moves->empty()) return false;
		
		bool found = false;
		unsigned int x = 0, y = 0;
		while(!found) {
			fputs("> ", stdout);
			
			int c = getchar();
			while(c == '\n') c = getchar();
			if(c == '?') {
				FloodFill::create(&board)->print();
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
		
		result = board;
		result.setCell(POSITION(x, y), player);
		return true;
    }
} }
