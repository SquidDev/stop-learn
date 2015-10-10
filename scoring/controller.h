#ifndef StopLearn_scoring_controller_H_
#define StopLearn_scoring_controller_H_

#include <stdint.h>
#include "../board.h"
#include "../position.h"

namespace StopLearn { namespace Scoring {
    enum class Controller : uint8_t { 
        None = 0,
        Occupied1 = 2,
        Occupied2 = 3,
        Owner1 = 4,
        Owner2 = 5,
        OwnerBoth = 1
    };
    
    // The maximum value is 5, which is 3 bits long
    #define CTRL_WIDTH 4
    // We could define the flag as 15, but we don't need the spare bit
    #define CTRL_FLAG 7
    
    class ControllerCell {
        public:
            const Position position;
            const Controller controller;
    };
    
    class ControllerMap {
        public:
            uint8_t counts[6] = {0};
            uint32_t rows[BOARD_SIZE] = {0};
            
            void print() const;
            inline Controller getCell(const Position position) const {
                return static_cast<Controller>((rows[position.y] >> (position.x * CTRL_WIDTH)) & CTRL_FLAG);
            }
            inline uint8_t getCount(const Controller controller) const {
                return counts[static_cast<uint8_t>(controller)];
            }
    };
    
    Controller cellController(const Board& board, const Position position);
    char controllerChar(const Controller controller);
} }
#endif
