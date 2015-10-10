#ifndef StopLearn_scoring_manhattan_H_
#define StopLearn_scoring_manhattan_H_

#include <stdint.h>
#include "../board.h"
#include "../position.h"
#include "controller.h"

namespace StopLearn { namespace Scoring {
    enum Distance : uint8_t {
        Undecided = 0,
        Both = 1,
        Player1 = 6,
        Player2 = 11,
    };
    
    // The maximum value is 15, which is 4 bits long
    #define MANHAT_WIDTH 4
    #define MANHAT_FLAG 15
    
    inline Distance playerToDistance(const Player player) {
        switch(player) {
            case Player::None: return Distance::Undecided;
            case Player::P1: return Distance::Player1;
            case Player::P2: return Distance::Player2;
        }
    }
    
    inline Distance decompose(const Distance distance) {
        if(distance >= Distance::Player2) {
			return Distance::Player2;
		} else if(distance >= Distance::Player1) {
			return Distance::Player1;
		} else if(distance >= Distance::Both) {
			return Distance::Both;
		} else {
			return Distance::Undecided;
		}
    }
    
    inline uint8_t toIndex(const Distance distance) {
        switch(decompose(distance)) {
            case Distance::Undecided: return 0;
            case Distance::Both: return 0;
            case Distance::Player1: return 1;
            case Distance::Player2: return 2;
        }
    }
    
    class ManhattanMap {
        private:
            uint8_t counts[3] = {0};
            uint32_t rows[BOARD_SIZE] = {0};
            Distance minDistance(const Position position) const;
        public:
            void print() const;
            std::unique_ptr<ControllerMap> toController() const;

            inline Distance getCell(const Position position) const {
                return static_cast<Distance>(rows[position.y] >> (position.x * MANHAT_WIDTH) & MANHAT_FLAG);
            }
            inline uint8_t getCount(const Distance distance) const {
                return counts[toIndex(distance)];
            }
            
            static int8_t score(const Board& board);
            static ManhattanMap create(const Board& board);
    };
} } 
#endif
