#include "moveGen.h"

#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

/* board representation */
#define WHITE 0
#define BLACK 1

#define WHITE_PAWN (2 * PAWN + WHITE)
#define BLACK_PAWN (2 * PAWN + BLACK)
#define WHITE_KNIGHT (2 * KNIGHT + WHITE)
#define BLACK_KNIGHT (2 * KNIGHT + BLACK)
#define WHITE_BISHOP (2 * BISHOP + WHITE)
#define BLACK_BISHOP (2 * BISHOP + BLACK)
#define WHITE_ROOK (2 * ROOK + WHITE)
#define BLACK_ROOK (2 * ROOK + BLACK)
#define WHITE_QUEEN (2 * QUEEN + WHITE)
#define BLACK_QUEEN (2 * QUEEN + BLACK)
#define WHITE_KING (2 * KING + WHITE)
#define BLACK_KING (2 * KING + BLACK)
#define EMPTY (BLACK_KING + 1)

#define NO_PIECE UINT8_MAX

#define NORTH 8
#define SOUTH -8
#define EAST -1
#define WEST 1

/** row from 0 - 7 */
inline uint8_t rank(uint8_t square)
{
    return square >> 3;
}

/** column from 0 - 7 */
inline uint8_t file(uint8_t square)
{
    return square & 7;
}

/**
 * Generates the pawn
 */
void &pawnGen(Board &board, MoveList list)
{
    bool whiteMove = board.isWhiteMove();
    // finds the proper place to get the pawns
    uint64_t pawnBoard = board.bitboard[WHITE_PAWN + (board.isWhiteMove() ? 0 : 1)];
    // if it's black, just rotate the board using byteswap and normally gen pawnMoves
    if (!whiteMove)
        pawnBoard = __builtin_bswap64(pawnBoard);

    for (int i = 0; i < 64; ++i)
    {
        if (!(pawnBoard & (1 << i)))
            continue;

        // we can double move this
        if (rank(i) == 1)
        {
        }
    }
}

// void knightMoveGen(unsigned long long board[], std::vector<unsigned long long[]> moves)
// {
//     // white

//     // black
// }

std::vector<Move> &moveGen(Board &board)
{
    MoveList list;

    pawnGen(board, moves);
}