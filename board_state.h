#ifndef __BOARD_STATE
#define __BOARD_STATE

#include <stdint.h>
#include "board.h"
#include "position.h"

namespace StopLearn {

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
        private:
            uint8_t counts[6] = {0};
            uint32_t rows[BOARD_SIZE] = {0};

            static bool floodFill(ControllerMap* map, const Controller owner, const Position position);
        public:

            void print() const;
            inline Controller getCell(const Position position) const {
                return static_cast<Controller>((rows[position.y] >> (position.x * CTRL_WIDTH)) & CTRL_FLAG);
            }
            inline uint8_t getCount(const Controller controller) const {
                return counts[static_cast<uint8_t>(controller)];
            }
            
            static std::unique_ptr<ControllerMap> create(const Board* board);
    };
    
    Controller cellController(const Board* board, const Position position);
    char controllerChar(const Controller controller);
}
#endif
