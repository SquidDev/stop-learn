#ifndef StopLearn_scoring_scoring_H_
#define StopLearn_scoring_scoring_H_

#include <stdint.h>
#include "../board.h"

namespace StopLearn { namespace Scoring {
    typedef int8_t (*BoardScorer)(const Board*);
} }

#endif
