#include "controller.h"

#include <stdlib.h>
#include <stdio.h>

using namespace std;
namespace StopLearn { namespace Scoring {
	Controller cellController(const Board* board, const Position position) {
		#define CHECK_PLAYER(player, cell) if(cell != Player::None) { if(player == Player::None) { player = cell; } else if(player != cell) { return Controller::OwnerBoth; } }

	    // Technically we don't need this.
	    Player player = board->getCell(position);
	    if(player != Player::None) return static_cast<Controller>(player);
	    
	    uint8_t x = position.x, y = position.y;
	
		if(y > 0) { // Above
			Player cell = board->getCell(POSITION(x, y - 1));
			if(cell != Player::None) {
			    CHECK_PLAYER(player, cell);
			} else if(y > 1) {
			    cell = board->getCell(POSITION(x, y - 2));
			    CHECK_PLAYER(player, cell);
			}
		}
	
		if(y < BOARD_SIZE - 1) { // Below
			Player cell = board->getCell(POSITION(x, y + 1));
			if(cell != Player::None) {
			    CHECK_PLAYER(player, cell);
			} else if(y < BOARD_SIZE - 2) {
			    cell = board->getCell(POSITION(x, y + 2));
			    CHECK_PLAYER(player, cell);
			}
		}
	
		if(x > 0) { // Left
			Player cell = board->getCell(POSITION(x - 1, y));
			if(cell != Player::None) {
			    CHECK_PLAYER(player, cell);
			} else if(x > 1) {
			    cell = board->getCell(POSITION(x - 2, y));
			    CHECK_PLAYER(player, cell);
			}
		}
	
		if(x < BOARD_SIZE - 1) { // Right
			Player cell = board->getCell(POSITION(x + 1, y));
			if(cell != Player::None) {
			    CHECK_PLAYER(player, cell);
			} else if(x < BOARD_SIZE - 2) {
			    cell = board->getCell(POSITION(x + 2, y));
			    CHECK_PLAYER(player, cell);
			}
		}
	
		return player == Player::None ? Controller::None : static_cast<Controller>(static_cast<uint8_t>(player) + 2);

		#undef CHECK_PLAYER
	}

	char controllerChar(Controller c) {
		switch(c) {
			case Controller::None:
				return ' ';
			case Controller::Occupied1:
				return 'X';
			case Controller::Occupied2:
				return 'O';
			case Controller::Owner1:
				return 'x';
			case Controller::Owner2:
				return 'o';
			case Controller::OwnerBoth:
				return '?';
		}
	}
	
	void ControllerMap::print() const {
		puts("  0 1 2 3 4 5 6 7");
		for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
			printf("%d|", y);
			for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
				putchar(controllerChar(getCell({x, y})));
				putchar('|');
			}
	
			putchar('\n');
		}
		
		for(uint8_t i = 0; i < 6; i++) {
			printf("'%c': %d, ", controllerChar(static_cast<Controller>(i)), counts[i]);
		}
		putchar('\n');
	}
} }
