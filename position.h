#ifndef __POSITION
#define __POSITION

#include <stdint.h>

#define POSITION(x, y) {static_cast<uint8_t>(x), static_cast<uint8_t>(y)}

namespace StopLearn {
    class Position {
        public:
            const uint8_t x;
            const uint8_t y;
            // static_assert(x >= 0 && x < BOARD_SIZE, "X must be between 0 and BOARD_SIZE");
            // static_assert(y >= 0 && y < BOARD_SIZE, "Y must be between 0 and BOARD_SIZE");
            
            void print() const;
         
    };
}
#endif
