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

// done from white's view
#define NORTH 8
#define SOUTH -8
#define EAST 1
#define WEST -1

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
 * converts a file and rank into a square
 */
inline uint8_t posToSquare(uint8_t file, uint8_t rank)
{
    return (rank << 3) + file;
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
    uint64_t pawnBoard = board.bitboard[board.isWhiteMove() ? 0 : 1];
    // if it's black, just rotate the board using byteswap and normally gen pawnMove

    // check for en passant
    if (board.canEnPassant())
    {
        // the the pawn we want to capture, NOT THE PAWN SQUARE FOR THE OTHER COLOR
        uint8_t captureSquare = board.enPassantSquare();
        if (board.isWhiteMove())
        {
            switch (file(captureSquare))
            {
            // leftmost file
            case 0:
                // check the right
                // if there is a pawn here
                if (pawnBoard & (1 << (captureSquare + EAST + SOUTH)))
                    list.insert(Move(captureSquare + EAST + SOUTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;

            // rightmost file
            case 7:
                // check the left
                // if there is a pawn here
                if (pawnBoard & (1 << (captureSquare + WEST + SOUTH)))
                    list.insert(Move(captureSquare + WEST + SOUTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;

            // in the middle, check both sides
            default:
                // check the left
                // if there is a pawn here
                if (pawnBoard & (1 << (captureSquare + WEST + SOUTH)))
                    list.insert(Move(captureSquare + WEST + SOUTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                // check the right
                // if there is a pawn here
                if (pawnBoard & (1 << (captureSquare + EAST + SOUTH)))
                    list.insert(Move(captureSquare + EAST + SOUTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;
            }
        }
        else
        {
            switch (file(captureSquare))
            {
            // leftmost file
            case 0:
                // check the right
                // if there is a pawn here
                if (pawnBoard & (1 << (captureSquare + EAST + NORTH)))
                    list.insert(Move(captureSquare + EAST + NORTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;

            // rightmost file
            case 7:
                // check the left
                // if there is a pawn here
                if (pawnBoard & (1 << (captureSquare + WEST + NORTH)))
                    list.insert(Move(captureSquare + WEST + NORTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;

            // in the middle, check both sides
            default:
                // check the left
                // if there is a pawn here
                if (pawnBoard & (1 << (captureSquare + WEST + NORTH)))
                    list.insert(Move(captureSquare + WEST + NORTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                // check the right
                // if there is a pawn here
                if (pawnBoard & (1 << (captureSquare + EAST + NORTH)))
                    list.insert(Move(captureSquare + EAST + NORTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;
            }
        }
    }

    // iterate backwards because it would optimize alpha beta
    // start at the 6th rank because its guaranteed that there won't be a pawn on the last rank
    if (whiteMove)
    {
        for (uint8_t i = 55; i >= 8; --i)
        {
            uint8_t fileNum = file(i);
            uint8_t rankNum = rank(i);
            // if there isn't a pawn here, continue
            if ((pawnBoard & 1 << i) == 0)
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
            if ((pawnBoard & 1 << i) == 0)
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

// these correspond to knightMaps and knightMoves
std::array<uint64_t, 8> knightMap = {0x7f7f7f7f7f7full, 0xfefefefefefeull, 0x7f7f7f7f7f7f0000ull, 0xfefefefefefe0000ull, 0x3f3f3f3f3f3f3full, 0xfcfcfcfcfcfcfcull, 0x3f3f3f3f3f3f3f00, 0xfcfcfcfcfcfcfc00};
std::array<int8_t, 8> knightMoves = {NORTH + NORTH + EAST, NORTH + NORTH + WEST, SOUTH + SOUTH + EAST, SOUTH + SOUTH + WEST, NORTH + EAST + EAST, NORTH + EAST + EAST, SOUTH + EAST + EAST, SOUTH + WEST + WEST};

/**
 * Generates all the knights
 */
void knightGen(Board &board, MoveList list)
{
    uint64_t knightBoard = board.bitboard[WHITE_KNIGHT + (board.isWhiteMove() ? 0 : 1)];

    // case for the middle 6x6, the knight can freely move
    // have two for optimization?
    // iterate from black side for optimization for alpha beta, because further you are in the chess board the more likely it is to be a better mov
    // iterator for the squares
    if (board.isWhiteMove())
    {
        for (uint8_t i = 63; i >= 0; --i)
        {
            // guard clause to prevent us from generating phantom moves
            if ((knightBoard & (1 << i)) == 0)
                continue;

            for (int moveType = 0; moveType < 8; ++moveType)
            {
                if (knightMap[moveType] & (1 << i))
                {
                    if (board.isBlackPieceAt(i + knightMoves[moveType]))
                    {
                        list.insert(Move(i, i + knightMoves[moveType], MoveFlag::CAPTURE));
                    }
                    else if (board.isWhitePieceAt(i + knightMoves[moveType]))
                    {
                        // don't push a move, there's another white piece there
                    }
                    // there's an empty square here, which is a quiet move
                    else
                    {
                        list.insert(Move(i, i + knightMoves[moveType], MoveFlag::QUIET_MOVE));
                    }
                }
            }
        }
    }
}

// first one is the allowed area the move is allowed to perform, and the second is the move itself
// in order it is: up, right, down, left, up & right, up & left, down & right, down & left
std::array<uint64_t, 8> kingMap = {0xffffffffffffffull, 0x7f7f7f7f7f7f7f7full, 0xffffffffffffff00ull, 0xfefefefefefefefeull, 0x7f7f7f7f7f7f7full, 0xfefefefefefefeull, 0x7f7f7f7f7f7f7f00ull, 0xfefefefefefefe00ull};
std::array<int8_t, 8> kingMove = {NORTH, EAST, SOUTH, WEST, NORTH + EAST, NORTH + WEST, SOUTH + EAST, SOUTH + WEST};

MoveList moveGen(Board &board)
{
    MoveList list;

    pawnGen(board, list);
    knightGen(board, list);

    return list;
}