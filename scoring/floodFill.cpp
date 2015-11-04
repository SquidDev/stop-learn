#include "floodFill.h"

#include <stdlib.h>
#include <stdio.h>
#include <forward_list>
#include <queue>

using namespace std;
namespace StopLearn { namespace Scoring { namespace FloodFill {
	uint64_t floodFill(ControllerMap* from, const uint64_t failures, const Controller owner, const Position position) {
		uint64_t visited = 0;

		queue<Position> positions;
		positions.push(position);

		while(!positions.empty()) {
			Position current = positions.front();
			uint8_t x = current.x, y = current.y;
			positions.pop();
			
			uint64_t flag = 1LL << (x + (BOARD_SIZE * y));
			
			// If we've hit a previously failed
			if((failures & flag) == flag) {
				return visited;
			}
			
			// If we've already visited, don't bother doing anything
			if((visited & flag) == flag) continue;
			
	
			Controller value = from->getCell(current);
			if(value == Controller::None) {
				// This is a "valid" cell, so set it as visited
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
			} else {
				// This is a player who isn't me, so abort.
				return visited;
			}
		}
		
		// Everything succeeded, so just return 0.
		return 0;
	}

	unique_ptr<ControllerMap> create(const Board& board) {
		ControllerMap controllers;
		
		forward_list<ControllerCell> toFill;
		for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
	    	for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
				Controller controller = cellController(board, {x, y});
				if(controller == Controller::Owner1 || controller == Controller::Owner2) {
					toFill.push_front({{x, y}, controller});
					
					if(x > 0) toFill.push_front({POSITION(x - 1, y), controller});
					if(x < BOARD_SIZE - 1) toFill.push_front({POSITION(x + 1, y), controller});
					if(y > 0) toFill.push_front({POSITION(x, y - 1), controller});
					if(y < BOARD_SIZE - 1) toFill.push_front({POSITION(x, y + 1), controller});
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

	int8_t score(const Board& board) {
		unique_ptr<ControllerMap> controller = create(board);
		return static_cast<int8_t>(controller->getCount(Controller::Owner1) - controller->getCount(Controller::Owner2));
	}
} } }
