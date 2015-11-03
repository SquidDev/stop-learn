#include "moving.h"

#include <stdint.h>
#include <stdio.h>
#include <memory>
#include <deque>
#include <algorithm>
#include "scoring/floodFill.h"
#include "scoring/manhattan.h"

using namespace std;
using namespace StopLearn::Scoring;

namespace StopLearn { namespace Moving {
    Board bestMove(const BoardScorer& scorer, const Board& board, const Player player) {
        unique_ptr<deque<Position>> moves = board.getMoves(player);
        random_shuffle(moves->begin(), moves->end());
        
        int8_t bestScore = 0;
        bool played = false;
        Board result = board;
        uint8_t x = 0, y = 0;
        
        for(const auto& move : *moves) {
			Board testBoard = board;
			testBoard.setCell(move, player);
			int8_t testScore = scorer(testBoard);
			
			if(!played || (player == Player::P1 ? testScore > bestScore : testScore < bestScore)) { // MinMax
				x = move.x; y = move.y;
				bestScore = testScore;
				result = testBoard;
				played = true; 
			}
		}
		
		printf("Player %d: Moving to %d, %d\n", static_cast<uint8_t>(player) - 1, x, y);
		
		return result;
    }
    
    BoardMover bestMove(const BoardScorer& scorer) {
    	using namespace std::placeholders;
    	return bind<Board, Board (const BoardScorer&, const Board&, const Player), const BoardScorer&>(bestMove, scorer, _1, _2);
    }
    
    int8_t applyMinmax(const Scoring::BoardScorer& scorer, const Board& board, const Player player, const uint8_t depth, unsigned int& accum) {
    	if(depth == 0) return scorer(board);
    	
        unique_ptr<deque<Position>> moves = board.getMoves(player);
        if(moves->empty()) return scorer(board);

    	int8_t bestScore = 0;
        bool played = false;
        
        uint8_t counter = 0;
        for(const auto& move : *moves) {
        	accum++;

			Board testBoard = board;
			testBoard.setCell(move, player);
			int8_t testScore = applyMinmax(scorer, testBoard, otherPlayer(player), depth - 1, accum);
			counter++;
			
			if(!played || (player == Player::P1 ? testScore > bestScore : testScore < bestScore)) {
				bestScore = testScore;
				played = true;
			}
		}
		
		return bestScore;
    }
    
    Board minmax(const Scoring::BoardScorer& scorer, const uint8_t depth, const Board& board, const Player player) {
        unique_ptr<deque<Position>> moves = board.getMoves(player);
        random_shuffle(moves->begin(), moves->end());
        
        int8_t bestScore = 0;
        bool played = false;
        Board result = board;
        uint8_t x = 0, y = 0;
        
        unsigned int counter = 0;
        for(const auto& move : *moves) {
			Board testBoard = board;
			testBoard.setCell(move, player);
			
			int8_t testScore = applyMinmax(scorer, testBoard, otherPlayer(player), depth, counter);
			
			if(!played || (player == Player::P1 ? testScore > bestScore : testScore < bestScore)) { // MinMax
				x = move.x; y = move.y;
				bestScore = testScore;
				result = testBoard;
				played = true;
			}
		}
		
		printf("Player %d: Moving to %d, %d\n", static_cast<uint8_t>(player) - 1, x, y);
		printf("After testing ~%d moves\n", counter);
		
		return result;
    }
    
    BoardMover minmax(const Scoring::BoardScorer& scorer, const uint8_t depth) {
    	return [=] (const Board& board, const Player player) {
    		return minmax(scorer, depth, board, player);
    	};
    }
    
    Position userSingleMove(const Board& board) {
    	while(true) {
	    	fputs("> ", stdout);
				
			int c = getchar();
			while(c == '\n') c = getchar();
			switch(c) {
				case 'f':
					FloodFill::create(board)->print();
					break;
				case 'm':
					ManhattanMap::create(board).print();
					break;
				case 's':
					printf("FloodFill: %d\n", FloodFill::score(board));
					printf("Manhattan: %d\n", ManhattanMap::score(board));
					break;
				case 'g':
					board.printSVG();
					break;
				default:
					ungetc(c, stdin);
					unsigned int x, y;
					if(scanf("%u,%u", &x, &y) && x < BOARD_SIZE && y < BOARD_SIZE) {
						return POSITION(x, y);
					}
					while(getchar() != '\n');
					puts("Expected x, y");
					break;
			}
    	}
    }
    
    Board userMove(const Board& board, const Player player) {
		unique_ptr<deque<Position>> moves = board.getMoves(player);
		
		while(true) {
			Position position = userSingleMove(board);
			uint8_t x = position.x, y = position.y;
			printf("Player %d: Moving to %d, %d\n", static_cast<uint8_t>(player) - 1, x, y);

			for(const auto& move : *moves) {
				if(move.x == x && move.y == y) {
					Board result = board;
					result.setCell(POSITION(x, y), player);
					return result;
				}
			}
			
			puts("Cannot move here");
		}
    }
} }
