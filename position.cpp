#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "position.h"

namespace StopLearn {
    void Position::print() const {
    	printf("(%d, %d", x, y);
    }
}
