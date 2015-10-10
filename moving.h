#ifndef StopLearn_moving_moving_H_
#define StopLearn_moving_moving_H_

#include "board.h"
#include <functional>
#include "scoring/scoring.h"

namespace StopLearn { namespace Moving {
    typedef std::function<Board (const Board&, const Player)> BoardMover;

    Board bestMove(const Scoring::BoardScorer& scorer, const Board& board, const Player player);
    
    BoardMover bestMove(const Scoring::BoardScorer& scorer);
    
    int8_t applyMinmax(const Scoring::BoardScorer& scorer, const Board& board, const Player player, const uint8_t depth, unsigned int& accum);
    
    Board minmax(const Scoring::BoardScorer& scorer, const uint8_t depth, const Board& board, const Player player);
    
    BoardMover minmax(const Scoring::BoardScorer& scorer, const uint8_t depth);
    
    Board userMove(const Board& board, const Player player);
    
    Position userSingleMove(const Board& board);
} }

#endif

