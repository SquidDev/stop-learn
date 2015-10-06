#include "floodFill.h"

#include <stdlib.h>
#include <stdio.h>
#include <forward_list>
#include <queue>

using namespace std;
namespace StopLearn { namespace Scoring { namespace FloodFill {
	uint64_t floodFill(ControllerMap* from, const uint64_t visited, const Controller owner, const Position position) {
		if(visited == 2) puts("Is 2!");
		queue<Position> positions;
		{
			uint8_t x = position.x, y = position.y;
			if(x > 0) positions.push(POSITION(x - 1, y));
			if(x < BOARD_SIZE - 1) positions.push(POSITION(x + 1, y));
			if(y > 0) positions.push(POSITION(x, y - 1));
			if(y < BOARD_SIZE - 1) positions.push(POSITION(x, y + 1));
		}
		
		// uint64_t visited = 0;
		
		while(!positions.empty()) {
			Position current = positions.front();
			uint8_t x = current.x, y = current.y;
			positions.pop();
			
			// uint64_t flag = 1LL << (x + (BOARD_SIZE * y));
			// if((visited & flag) == flag) continue;
	
			Controller value = from->getCell(current);
			if(value == Controller::None) {
				// visited |= flag;
				
				from->rows[y] |= static_cast<uint32_t>(owner) << (x * CTRL_WIDTH);
				from->counts[static_cast<uint8_t>(Controller::None)]--;
				from->counts[static_cast<uint8_t>(owner)]++;
				
				// If we've overridden this value then scan for others
				if(x > 0) positions.push(POSITION(x - 1, y));
				if(x < BOARD_SIZE - 1) positions.push(POSITION(x + 1, y));
				if(y > 0) positions.push(POSITION(x, y - 1));
				if(y < BOARD_SIZE - 1) positions.push(POSITION(x, y + 1));
			} else if(value == owner || value == static_cast<Controller>(static_cast<uint8_t>(owner) - 2)) {
				// visited |= flag;
			} else {
				return 1;
			}
		}
		
		return 0;
	}

	unique_ptr<ControllerMap> create(const Board* board) {
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
		
		uint64_t failed = 0;
		for(const auto &item : toFill) {
			ControllerMap tempControllers = controllers;
			uint64_t visited = floodFill(&tempControllers, failed, item.controller, item.position);
			
			// Only cache visits which failed
			if(visited == 0) {
				controllers = tempControllers;
			} else {
				failed |= visited;
			}
		}
		
		return unique_ptr<ControllerMap>(new ControllerMap(controllers));
	}

	int8_t score(const Board* board) {
		unique_ptr<ControllerMap> controller = create(board);
		return static_cast<int8_t>(controller->getCount(Controller::Owner1) - controller->getCount(Controller::Owner2));
	}
} } }
