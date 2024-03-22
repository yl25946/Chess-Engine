#include "evaluation.h"

/**
 * Basic piece coutning
 */
int32_t eval(BoardState &board)
{
    std::array<int32_t, 10> piece_weights = {1, -1, 3, -3, 3, -3, 5, -5, 8, -8};

    int32_t eval_value = 0;

    for (int i = 0; i <= 9; ++i)
    {
        eval_value += piece_weights[i] * __builtin_popcountll(board.bitboard[i]);
    }

    return board.isWhiteMove() ? eval_value : -1 * eval_value;
};