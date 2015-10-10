#ifndef StopLearn_scoring_floodFill_H_
#define StopLearn_scoring_floodFill_H_

#include <stdint.h>
#include "../board.h"
#include "../position.h"
#include "controller.h"

namespace StopLearn { namespace Scoring { namespace FloodFill {
    uint64_t floodFill(ControllerMap* map, const uint64_t visited, const Controller owner, const Position position);
    std::unique_ptr<ControllerMap> create(const Board& board);
    
    int8_t score(const Board& board);
} } }
#endif
