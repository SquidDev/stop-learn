#ifndef StopLearn_moving_moving_H_
#define StopLearn_moving_moving_H_

#include "board.h"
#include "scoring/scoring.h"

namespace StopLearn { namespace Moving {
    bool bestMove(const Scoring::BoardScorer scorer, const Board& board, const Player player, Board& result);
    
    bool userMove(const Board& board, const Player player, Board& result);
} }

#endif

