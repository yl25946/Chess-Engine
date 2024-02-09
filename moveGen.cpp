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
 * Adds all the promotions
 */
void addPromotionMoves(MoveList list, uint8_t currSquare, uint8_t promotionSquare, bool capture)
{
    list.insert(Move(currSquare, promotionSquare, capture ? MoveFlag::QUEEN_PROMOTION : QUEEN_PROMOTION_CAPTURE));
    list.insert(Move(currSquare, promotionSquare, capture ? MoveFlag::ROOK_PROMOTION : ROOK_PROMOTION_CAPTURE));
    list.insert(Move(currSquare, promotionSquare, capture ? MoveFlag::BISHOP_PROMOTION : BISHOP_PROMOTION_CAPTURE));
    list.insert(Move(currSquare, promotionSquare, capture ? MoveFlag::BISHOP_PROMOTION : BISHOP_PROMOTION_CAPTURE));
}

/**
 * Generates the pawn moves
 */
void pawnGen(const Board &board, MoveList list)
{
    bool whiteMove = board.isWhiteMove();
    // finds the proper place to get the pawns
    uint64_t pawnBoard = board.bitboard[WHITE_PAWN + (board.isWhiteMove() ? 0 : 1)];
    // if it's black, just rotate the board using byteswap and normally gen pawnMove

    // iterate backwards because it would optimize alpha beta
    // start at the 6th rank because its guaranteed that there won't be a pawn on the last rank
    if (whiteMove)
    {
        for (uint8_t i = 55; i >= 8; --i)
        {
            uint8_t fileNum = file(i);
            uint8_t rankNum = rank(i);
            // if there isn't a pawn here, continue
            if (pawnBoard & 1 << i)
                continue;

            // check promotions
            if (rankNum == 6)
            {
                // move forward
                if (!board.isPieceAt(i + NORTH))
                {
                    addPromotionMoves(list, i, i + NORTH, false);
                }
                // if we can take to the left
                if ((fileNum != 0) && board.isBlackPieceAt(NORTH + EAST))
                {
                    addPromotionMoves(list, i, i + NORTH + EAST, true);
                }
                if ((fileNum != 7) && board.isBlackPieceAt(i + NORTH + WEST))
                {
                    addPromotionMoves(list, i, i + NORTH + WEST, true);
                }
                // we got all the possibilities
                continue;
            }

            // pawn capture to the left available
            if ((fileNum != 0) && board.isPieceAt(i + NORTH + EAST))
                list.insert(Move(i, i + NORTH + EAST, MoveFlag::CAPTURE));

            // pawn capture to the right
            if ((fileNum != 7) && board.isBlackPieceAt(i + NORTH + WEST))
                list.insert(Move(i, i + NORTH + WEST, MoveFlag::CAPTURE));

            // we can double move this
            if (rankNum == 1)
            {
                if (!board.isPieceAt(i + NORTH) && !board.isPieceAt(i + NORTH + NORTH))
                    list.insert(Move(i, i + NORTH + NORTH, MoveFlag::DOUBLE_PAWN_PUSH));
            }

            // just move the pawn forward
            if (!board.isPieceAt(i + NORTH))
                list.insert(Move(i, i + NORTH, MoveFlag::QUIET_MOVE));
        }
    }

    else
    {
        for (uint8_t i = 8; i <= 55; ++i)
        {
            uint8_t fileNum = file(i);
            uint8_t rankNum = rank(i);
            // if there isn't a pawn on this square
            if (pawnBoard & 1 << i)
                continue;

            // check promotions
            if (rankNum == 1)
            {
                // move forward
                if (!board.isPieceAt(i + SOUTH))
                {
                    addPromotionMoves(list, i, i + SOUTH, false);
                }
                // if we can take to the left
                if ((fileNum != 0) && board.isBlackPieceAt(SOUTH + EAST))
                {
                    addPromotionMoves(list, i, i + SOUTH + EAST, true);
                }
                if ((fileNum != 7) && board.isBlackPieceAt(i + SOUTH + WEST))
                {
                    addPromotionMoves(list, i, i + SOUTH + WEST, true);
                }
                // we got all the possibilities
                continue;
            }

            // pawn capture to the left available
            if ((fileNum != 0) && board.isPieceAt(i + SOUTH + EAST))
                list.insert(Move(i, i + SOUTH + EAST, MoveFlag::CAPTURE));

            // pawn capture to the right
            if ((fileNum != 7) && board.isBlackPieceAt(i + SOUTH + WEST))
                list.insert(Move(i, i + SOUTH + WEST, MoveFlag::CAPTURE));

            // we can double move this
            if (rankNum == 6)
            {
                if (!board.isPieceAt(i + SOUTH) && !board.isPieceAt(i + SOUTH + SOUTH))
                    list.insert(Move(i, i + SOUTH + SOUTH, MoveFlag::DOUBLE_PAWN_PUSH));
            }

            // just move the pawn forward
            if (!board.isPieceAt(i + SOUTH))
                list.insert(Move(i, i + SOUTH, MoveFlag::QUIET_MOVE));
        }
    }
}

void knightGen(Board &board, MoveList list)
{
    // white

    // black
}

MoveList moveGen(Board &board)
{
    MoveList list;

    pawnGen(board, list);
    knightGen(board, list);

    return list;
}