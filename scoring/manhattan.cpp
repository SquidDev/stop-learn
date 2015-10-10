#include "manhattan.h"

#include <stdlib.h>
#include <stdio.h>
#include <forward_list>
#include <queue>

using namespace std;
namespace StopLearn { namespace Scoring {
	void ManhattanMap::print() const {
		puts("  0  1  2  3  4  5  6  7");
		for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
			printf("%d|", y);
			for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
				Distance distance = getCell({x, y});
				if(distance == Distance::Undecided) {
					putchar(' '); putchar(' ');
				} else {
					Distance decomp = decompose(distance);
					switch(decomp) {
						case Distance::Both: putchar('?'); break;
						case Distance::Player1: putchar('X'); break;
						case Distance::Player2: putchar('O'); break;
						case Distance::Undecided: putchar('.');
					}
					printf("%d", distance - decomp);
				}
				putchar('|');
			}
	
			putchar('\n');
		}

		printf("'?': %d, ", counts[0]);
		printf("'X': %d, ", counts[1]);
		printf("'O': %d", counts[2]);
		putchar('\n');
	}
	
	Distance ManhattanMap::minDistance(const Position position) const {
		#define CHECK_PLAYER { \
			Distance tempPlayer;  \
			uint8_t tempDistance; \
			tempPlayer = decompose(cell); \
			if(tempPlayer != Distance::Undecided) { \
				tempDistance = cell - tempPlayer + 1; \
				if(tempDistance < distance) { \
					player = tempPlayer; distance = tempDistance; \
				} else if(tempDistance == distance) { \
					if(player == Distance::Undecided) { player = tempPlayer; } \
					else if(player != tempPlayer) { player = Distance::Both; } \
				} \
			} \
		}
			
	    uint8_t x = position.x, y = position.y;
	
		Distance player = Distance::Undecided;
		uint8_t distance = 255;
		if(y > 0) {
			Distance cell = getCell(POSITION(x, y - 1));
			if(cell != Distance::Undecided) {
			    CHECK_PLAYER
			} else if(y > 1) {
			    cell = getCell(POSITION(x, y - 2));
			    CHECK_PLAYER
			}
		}
	
		if(y < BOARD_SIZE - 1) { // Below
			Distance cell = getCell(POSITION(x, y + 1));
			if(cell != Distance::Undecided) {
			    CHECK_PLAYER
			} else if(y < BOARD_SIZE - 2) {
			    cell = getCell(POSITION(x, y + 2));
			    CHECK_PLAYER
			}
		}
	
		if(x > 0) { // Left
			Distance cell = getCell(POSITION(x - 1, y));
			if(cell != Distance::Undecided) {
			    CHECK_PLAYER
			} else if(x > 1) {
			    cell = getCell(POSITION(x - 2, y));
			    CHECK_PLAYER
			}
		}
	
		if(x < BOARD_SIZE - 1) { // Right
			Distance cell = getCell(POSITION(x + 1, y));
			if(cell != Distance::Undecided) {
			    CHECK_PLAYER
			} else if(x < BOARD_SIZE - 2) {
			    cell = getCell(POSITION(x + 2, y));
			    CHECK_PLAYER
			}
		}
	
		if(distance > 4) return Distance::Undecided;
		return player == Distance::Undecided ? player : static_cast<Distance>(player + distance);
		
		#undef CHECK_PLAYER
	}
	
	ManhattanMap ManhattanMap::create(const Board& board) {
		ManhattanMap map;
		for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
			for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
				Distance distance = playerToDistance(board.getCell({x, y}));
				if(distance != Distance::Undecided) {
					map.rows[y] |= static_cast<uint32_t>(distance) << (x * MANHAT_WIDTH);
					map.counts[toIndex(distance)]++;
				}
			}
		}

		while(true) { 
			bool changed = false;
			ManhattanMap newMap = map;
			
			for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
				for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
					Distance distance = map.getCell({x, y});
					if(distance == Distance::Undecided) {
						distance = map.minDistance({x, y});
						if(distance != Distance::Undecided) {
							newMap.rows[y] |= static_cast<uint32_t>(distance) << (x * MANHAT_WIDTH);
							newMap.counts[toIndex(distance)]++;
							changed = true;
						}
					}
				}
			}
			
			if(changed) {
				map = newMap;
			} else {
				break;
			}
		}
		
		return map;
	}
	
	int8_t ManhattanMap::score(const Board& board) {
		ManhattanMap map = create(board);
		return static_cast<int8_t>(map.getCount(Distance::Player1) - map.getCount(Distance::Player2));
	}
} }
