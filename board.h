#ifndef StopLearn_board_H_
#define StopLearn_board_H_

#include <stdint.h>
#include <stdbool.h>
#include <forward_list>
#include "position.h"

#define COL_FLAG 3
#define BOARD_SIZE 8

namespace StopLearn {
	enum class Player : uint8_t { None  = 0, P1 = 2, P2 = 3 };
	
	class Board final {
		private:
			uint16_t rows[BOARD_SIZE] = {};
			bool canMove(const Position position, const Player player) const;
		public:
			/** Sets players in opposite corners */
			void setup();
	
			/** Get player at a cell */
			inline Player getCell(const Position position) const {
				return static_cast<Player>((rows[position.y] >> (position.x * 2)) & COL_FLAG);
			}
			/** Sets a player at a cell */
			inline void setCell(const Position position, const Player player) {
				rows[position.y] |= static_cast<uint8_t>(player) << (position.x * 2);
			}
			
			/** Get all possible moves */
			std::unique_ptr<std::forward_list<Position>> getMoves(const Player player) const;
			
			/** Print the board */
			void print() const;
	};
	
	inline Player otherPlayer(Player player)  { return static_cast<Player>(1 ^ static_cast<uint8_t>(player)); }

}
#endif
