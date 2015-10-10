#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "board.h"

using namespace std;
namespace StopLearn {
	void Board::setup() {
		rows[0] = static_cast<uint8_t>(Player::P1) << ((BOARD_SIZE - 1) * 2);
		rows[BOARD_SIZE - 1] = static_cast<uint8_t>(Player::P2);
	}
	
	void Board::print() const {
		printf("  0 1 2 3 4 5 6 7\n");
		for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
			printf("%d|", y);
			for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
				switch(getCell({x, y})) {
					case Player::None:
						putchar(' ');
						break;
					case Player::P1:
						putchar('X');
						break;
					case Player::P2:
						putchar('0');
						break;
				}
				putchar('|');
			}
	
			putchar('\n');
		}
	}
	
	unique_ptr<forward_list<Position>> Board::getMoves(const Player player) const {
		forward_list<Position>* positions = new forward_list<Position>();
	
		for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
			for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
				Position position = {x, y};
				if(canMove(position, player)) {
					positions->push_front(position);
				}
			}
		}
	
		return unique_ptr<forward_list<Position>>(positions);
	}
	
	bool Board::canMove(const Position position, const Player player) const {
		// Only search in empty cells
		if(getCell(position) != Player::None) return false;
		
		uint8_t x = position.x, y = position.y;
	
		if(y > 0) { // Above
			Player cell = getCell(POSITION(x, y - 1));
			if(
				(cell == player) ||
				(cell == Player::None && y > 1 && getCell(POSITION(x, y - 2)) == player) // Empty above and player two above
			) {
				return true;
			}
		}
	
		if(y < BOARD_SIZE - 1) { // Below
			Player cell = getCell(POSITION(x, y + 1));
			if(
				(cell == player) ||
				(cell == Player::None && y < BOARD_SIZE - 2 && getCell(POSITION(x, y + 2)) == player)
			) {
				return true;
			}
		}
	
		if(x > 0) { // Left
			Player cell = getCell(POSITION(x - 1, y));
			if(
				(cell == player) ||
				(cell == Player::None && x > 1 && getCell(POSITION(x - 2, y)) == player)
			) {
				return true;
			}
		}
	
		if(x < BOARD_SIZE - 1) { // Right
			Player cell = getCell(POSITION(x + 1, y));
			if(
				(cell == player) ||
				(cell == Player::None && x < BOARD_SIZE - 2 && getCell(POSITION(x + 2, y)) == player)
			) {
				return true;
			}
		}
	
		return false;
	}
	
	bool Board::canMove(const Player player) const {
		for(uint8_t x = 0; x < BOARD_SIZE; ++x) {
			for(uint8_t y = 0; y < BOARD_SIZE; ++y) {
				if(canMove({x, y}, player)) {
					return true;
				}
			}
		}
		
		return false;
	}
}
