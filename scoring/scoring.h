#ifndef StopLearn_scoring_scoring_H_
#define StopLearn_scoring_scoring_H_

#include <stdint.h>
#include <functional>
#include "../board.h"

namespace StopLearn { namespace Scoring {
    typedef std::function<int8_t (const Board& board)> BoardScorer;
} }

#endif
