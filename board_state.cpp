#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <forward_list>
#include <queue>
#include "board.h"
#include "board_state.h"
#include "position.h"

using namespace std;
namespace StopLearn {
	// Horrible macro.
	#define CHECK_PLAYER(player, cell) if(cell != Player::None) { if(player == Player::None) { player = cell; } else if(player != cell) { return Controller::OwnerBoth; } }
	
	Controller cellController(const Board* board, const Position position) {
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
	}
	
	#undef CHECK_PLAYER
	
	bool ControllerMap::floodFill(ControllerMap* from, const Controller owner, const Position position) {
		queue<Position> positions;
		{
			uint8_t x = position.x, y = position.y;
			if(x > 0) positions.push(POSITION(x - 1, y));
			if(x < BOARD_SIZE - 1) positions.push(POSITION(x + 1, y));
			if(y > 0) positions.push(POSITION(x, y - 1));
			if(y < BOARD_SIZE - 1) positions.push(POSITION(x, y + 1));
		}
		
		uint64_t visited = 0;
		
		while(!positions.empty()) {
			Position current = positions.front();
			uint8_t x = current.x, y = current.y;
			positions.pop();
			
			uint64_t flag = 1LL << (x + (BOARD_SIZE * y));
			if((visited & flag) == flag) continue;
	
			Controller value = from->getCell(current);
			if(value == Controller::None) {
				visited |= flag;
				
				from->rows[y] |= static_cast<uint32_t>(owner) << (x * CTRL_WIDTH);
				from->counts[static_cast<uint8_t>(Controller::None)]--;
				from->counts[static_cast<uint8_t>(owner)]++;
				
				// If we've overridden this value then scan for others
				if(x > 0) positions.push(POSITION(x - 1, y));
				if(x < BOARD_SIZE - 1) positions.push(POSITION(x + 1, y));
				if(y > 0) positions.push(POSITION(x, y - 1));
				if(y < BOARD_SIZE - 1) positions.push(POSITION(x, y + 1));
			} else if(value == owner || value == static_cast<Controller>(static_cast<uint8_t>(owner) - 2)) {
				visited |= flag;
			} else {
				return false;
			}
		}
		
		return true;
	}

	unique_ptr<ControllerMap> ControllerMap::create(const Board* board) {
		ControllerMap controllers;
		
		forward_list<ControllerCell> toFill;
		for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
	    	for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
				Controller controller = cellController(board, {x, y});
				if(controller == Controller::Owner1 || controller == Controller::Owner2) {
					toFill.push_front({{x, y}, controller});
				}
	
				controllers.rows[y] |= static_cast<uint32_t>(controller) << (x * CTRL_WIDTH);
				controllers.counts[static_cast<uint8_t>(controller)]++;
			}
		}
		
		for(const auto &item : toFill) {
			ControllerMap tempControllers = controllers;
			if(floodFill(
				&tempControllers, 
				item.controller,
				item.position
			)) {
				controllers = tempControllers;
			}
		}
		
		return unique_ptr<ControllerMap>(new ControllerMap(controllers));
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
}
