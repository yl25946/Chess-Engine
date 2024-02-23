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

// defines the index to all the white/black pieces index in the bitboard
#define ALL_WHITE_PIECES 12
#define ALL_BLACK_PIECES 13

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
void pawnGen(const Board &board, MoveList &list)
{
    // finds the proper place to get the pawns
    uint8_t pawnBitboardIndex = board.isWhiteMove() ? 0 : 1;

    Board copy = board;
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
                if (copy.bitboard[pawnBitboardIndex] & (1 << (captureSquare + EAST + SOUTH)))
                    list.insert(Move(captureSquare + EAST + SOUTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;

            // rightmost file
            case 7:
                // check the left
                // if there is a pawn here
                if (copy.bitboard[pawnBitboardIndex] & (1 << (captureSquare + WEST + SOUTH)))
                    list.insert(Move(captureSquare + WEST + SOUTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;

            // in the middle, check both sides
            default:
                // check the left
                // if there is a pawn here
                if (copy.bitboard[pawnBitboardIndex] & (1 << (captureSquare + WEST + SOUTH)))
                    list.insert(Move(captureSquare + WEST + SOUTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                // check the right
                // if there is a pawn here
                if (copy.bitboard[pawnBitboardIndex] & (1 << (captureSquare + EAST + SOUTH)))
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
                if (copy.bitboard[pawnBitboardIndex] & (1 << (captureSquare + EAST + NORTH)))
                    list.insert(Move(captureSquare + EAST + NORTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;

            // rightmost file
            case 7:
                // check the left
                // if there is a pawn here
                if (copy.bitboard[pawnBitboardIndex] & (1 << (captureSquare + WEST + NORTH)))
                    list.insert(Move(captureSquare + WEST + NORTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;

            // in the middle, check both sides
            default:
                // check the left
                // if there is a pawn here
                if (copy.bitboard[pawnBitboardIndex] & (1 << (captureSquare + WEST + NORTH)))
                    list.insert(Move(captureSquare + WEST + NORTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                // check the right
                // if there is a pawn here
                if (copy.bitboard[pawnBitboardIndex] & (1 << (captureSquare + EAST + NORTH)))
                    list.insert(Move(captureSquare + EAST + NORTH, captureSquare, MoveFlag::EN_PASSANT_CAPTURE));
                break;
            }
        }
    }

    // iterate backwards because it would optimize alpha beta
    // start at the 6th rank because its guaranteed that there won't be a pawn on the last rank
    if (board.isWhiteMove())
    {
        while (copy.bitboard[pawnBitboardIndex])
        {
            uint8_t square = copy.popBitboard(pawnBitboardIndex);
            uint8_t fileNum = file(square);
            uint8_t rankNum = rank(square);

            // check promotions
            if (rankNum == 6)
            {
                // move forward
                if (!board.isPieceAt(square + NORTH))
                {
                    addPromotionMoves(list, square, square + NORTH, false);
                }
                // if we can take to the left
                if ((fileNum != 0) && board.isBlackPieceAt(NORTH + EAST))
                {
                    addPromotionMoves(list, square, square + NORTH + EAST, true);
                }
                if ((fileNum != 7) && board.isBlackPieceAt(square + NORTH + WEST))
                {
                    addPromotionMoves(list, square, square + NORTH + WEST, true);
                }
                // we got all the possibilities
                continue;
            }

            // pawn capture to the left available
            if ((fileNum != 0) && board.isPieceAt(square + NORTH + EAST))
                list.insert(Move(square, square + NORTH + EAST, MoveFlag::CAPTURE));

            // pawn capture to the right
            if ((fileNum != 7) && board.isBlackPieceAt(square + NORTH + WEST))
                list.insert(Move(square, square + NORTH + WEST, MoveFlag::CAPTURE));

            // we can double move this
            if (rankNum == 1)
            {
                if (!board.isPieceAt(square + NORTH) && !board.isPieceAt(square + NORTH + NORTH))
                    list.insert(Move(square, square + NORTH + NORTH, MoveFlag::DOUBLE_PAWN_PUSH));
            }

            // just move the pawn forward
            if (!board.isPieceAt(square + NORTH))
                list.insert(Move(square, square + NORTH, MoveFlag::QUIET_MOVE));
        }
    }

    else
    {
        while (copy.bitboard[pawnBitboardIndex])
        {
            uint8_t square = copy.popBitboard(pawnBitboardIndex);
            uint8_t fileNum = file(square);
            uint8_t rankNum = rank(square);
            // if there isn't a pawn on this square

            // check promotions
            if (rankNum == 1)
            {
                // move forward
                if (!board.isPieceAt(square + SOUTH))
                {
                    addPromotionMoves(list, square, square + SOUTH, false);
                }
                // if we can take to the left
                if ((fileNum != 0) && board.isBlackPieceAt(SOUTH + EAST))
                {
                    addPromotionMoves(list, square, square + SOUTH + EAST, true);
                }
                if ((fileNum != 7) && board.isBlackPieceAt(square + SOUTH + WEST))
                {
                    addPromotionMoves(list, square, square + SOUTH + WEST, true);
                }
                // we got all the possibilities
                continue;
            }

            // pawn capture to the left available
            if ((fileNum != 0) && board.isPieceAt(square + SOUTH + EAST))
                list.insert(Move(square, square + SOUTH + EAST, MoveFlag::CAPTURE));

            // pawn capture to the right
            if ((fileNum != 7) && board.isBlackPieceAt(square + SOUTH + WEST))
                list.insert(Move(square, square + SOUTH + WEST, MoveFlag::CAPTURE));

            // we can double move this
            if (rankNum == 6)
            {
                if (!board.isPieceAt(square + SOUTH) && !board.isPieceAt(square + SOUTH + SOUTH))
                    list.insert(Move(square, square + SOUTH + SOUTH, MoveFlag::DOUBLE_PAWN_PUSH));
            }

            // just move the pawn forward
            if (!board.isPieceAt(square + SOUTH))
                list.insert(Move(square, square + SOUTH, MoveFlag::QUIET_MOVE));
        }
    }
}

// these correspond to knightMaps and knightMoves
std::array<uint64_t, 8> knightMap = {0x7f7f7f7f7f7full, 0xfefefefefefeull, 0x7f7f7f7f7f7f0000ull, 0xfefefefefefe0000ull, 0x3f3f3f3f3f3f3full, 0xfcfcfcfcfcfcfcull, 0x3f3f3f3f3f3f3f00, 0xfcfcfcfcfcfcfc00};
std::array<int8_t, 8> knightMoves = {NORTH + NORTH + EAST, NORTH + NORTH + WEST, SOUTH + SOUTH + EAST, SOUTH + SOUTH + WEST, NORTH + EAST + EAST, NORTH + EAST + EAST, SOUTH + EAST + EAST, SOUTH + WEST + WEST};

/**
 * Generates all the knights
 */
void knightGen(Board &board, MoveList &list)
{
    // index where we can access the correct bitboard
    uint64_t knightBitboardIndex = 2 + (board.isWhiteMove() ? 0 : 1);

    Board copy = board;

    // case for the middle 6x6, the knight can freely move
    // have two for optimization?
    // iterate from black side for optimization for alpha beta, because further you are in the chess board the more likely it is to be a better mov
    // iterator for the squares
    if (board.isWhiteMove())
    {
        while (copy.bitboard[knightBitboardIndex])
        {
            uint8_t square = copy.popBitboard(knightBitboardIndex);
            for (int moveType = 0; moveType < 8; ++moveType)
            {
                // within the bounds
                if (knightMap[moveType] & (1 << square))
                {
                    if (board.isBlackPieceAt(square + knightMoves[moveType]))
                    {
                        list.insert(Move(square, square + knightMoves[moveType], MoveFlag::CAPTURE));
                    }
                    else if (board.isWhitePieceAt(square + knightMoves[moveType]))
                    {
                        // don't push a move, there's another white piece there
                    }
                    // there's an empty square here, which is a quiet move
                    else
                    {
                        list.insert(Move(square, square + knightMoves[moveType], MoveFlag::QUIET_MOVE));
                    }
                }
            }
        }
    }
    // it is the black moveGen
    else
    {
        while (copy.bitboard[knightBitboardIndex])
        {
            uint8_t square = copy.popBitboard(knightBitboardIndex);
            for (int moveType = 0; moveType < 8; ++moveType)
            {
                // within the bounds
                if (knightMap[moveType] & (1 << square))
                {
                    if (board.isWhitePieceAt(square + knightMoves[moveType]))
                    {
                        list.insert(Move(square, square + knightMoves[moveType], MoveFlag::CAPTURE));
                    }
                    else if (board.isBlackPieceAt(square + knightMoves[moveType]))
                    {
                        // don't push a move, there's another black piece there
                    }
                    // there's an empty square here, which is a quiet move
                    else
                    {
                        list.insert(Move(square, square + knightMoves[moveType], MoveFlag::QUIET_MOVE));
                    }
                }
            }
        }
    }
}

// first one is the allowed area the move is allowed to perform, and the second is the move itself
// in order it is: up, right, down, left, up & right, up & left, down & right, down & left
std::array<uint64_t, 8> kingMap = {0xffffffffffffffull, 0x7f7f7f7f7f7f7f7full, 0xffffffffffffff00ull, 0xfefefefefefefefeull, 0x7f7f7f7f7f7f7full, 0xfefefefefefefeull, 0x7f7f7f7f7f7f7f00ull, 0xfefefefefefefe00ull};
std::array<int8_t, 8> kingMoves = {NORTH, EAST, SOUTH, WEST, NORTH + EAST, NORTH + WEST, SOUTH + EAST, SOUTH + WEST};

void kingGen(Board &board, MoveList &list)
{
    // index of the bitboard we want to access
    uint8_t kingBitboardIndex = 10 + (board.isWhiteMove() ? 0 : 1);
    // gets it manually, because thats (probably?) faster, and we are guaranteed to get a king, so there's no need to iterate like say a queen, which can be taken
    uint8_t square = __builtin_ffsll(board.bitboard[kingBitboardIndex]) - 1;

    if (board.isWhiteMove())
    {
        for (int moveType = 0; moveType < 8; ++moveType)
        {
            // within the bounds
            if (kingMap[moveType] & (1 << square))
            {
                if (board.isBlackPieceAt(square + kingMoves[moveType]))
                {
                    list.insert(Move(square, square + kingMoves[moveType], MoveFlag::CAPTURE));
                }
                else if (board.isWhitePieceAt(square + kingMoves[moveType]))
                {
                    // don't push a move, there's another white piece there
                }
                // there's an empty square here, which is a quiet move
                else
                {
                    list.insert(Move(square, square + kingMoves[moveType], MoveFlag::QUIET_MOVE));
                }
            }
        }
    }

    // it is the black moveGen
    else
    {
        for (int moveType = 0; moveType < 8; ++moveType)
        {
            // within the bounds
            if (kingMap[moveType] & (1 << square))
            {
                if (board.isWhitePieceAt(square + kingMoves[moveType]))
                {
                    list.insert(Move(square, square + kingMoves[moveType], MoveFlag::CAPTURE));
                }
                else if (board.isBlackPieceAt(square + kingMoves[moveType]))
                {
                    // don't push a move, there's another black piece there
                }
                // there's an empty square here, which is a quiet move
                else
                {
                    list.insert(Move(square, square + kingMoves[moveType], MoveFlag::QUIET_MOVE));
                }
            }
        }
    }
}

/**
 * white kingside castle, white queenside castle, black queenside castle, black queenside castle
 * If you map this on all pieces bitboard and it doesn't return 0, then you can't castle
 */
std::array<uint64_t, 4> castleMap = {0x60ull, 0xeull, 0x6000000000000000ull, 0xe00000000000000ull};
std::array<int8_t, 4> castleMoves = {EAST + EAST, WEST + WEST, EAST + EAST, WEST + WEST};
std::array<uint8_t, 4> castleStartSquare = {
    4,
    4,
    60,
    60,
};
std::array<uint8_t, 4> castleEndSquare = {
    6,
    2,
    62,
    58,
};
std::array<MoveFlag, 4> castleMoveFlags = {MoveFlag::KINGSIDE_CASTLE, MoveFlag::QUEENSIDE_CASTLE, MoveFlag::KINGSIDE_CASTLE, MoveFlag::QUEENSIDE_CASTLE};

/**
 * Generates all the moves for castling
 * DOES NOT CHECK IF THEY ARE IN CHECK, CASTLING THROUGH CHECK, OR CASTLING INTO CHECK
 */
void castleGen(Board &board, MoveList &list)
{
    uint64_t allPiecesBitboard = board.bitboard[ALL_WHITE_PIECES] | board.bitboard[ALL_BLACK_PIECES];
    // if we have black then we go to different part of the array
    uint8_t offset = board.isWhiteMove() ? 0 : 2;
    std::array<bool, 4> castlePerms = {board.canWhiteKingCastle(), board.canWhiteQueenCastle(), board.canBlackKingCastle(), board.canBlackQueenCastle()};
    for (int i = 0; i < 2; ++i)
    {
        if (castlePerms[i + offset] && (allPiecesBitboard & castleMap[i + offset] == 0))
            list.insert(Move(castleStartSquare[i + offset], castleEndSquare[i + offset], castleMoveFlags[i + offset]));
    }
}

/**
 * Generate all the bishop moves
 */
void bishopGen(Board &board, MoveList &list)
{
}

/**
 * Generates all the rook moves
 */
void rookGen(Board &board, MoveList &list)
{
    // index where we can access the correct bitboard
    uint64_t rookBitboardIndex = 6 + (board.isWhiteMove() ? 0 : 1);

    Board copy = board;

    // literally iterates until it hits a black/white piece, then stops
    if (board.isWhiteMove())
    {
        while (copy.bitboard[rookBitboardIndex])
        {
            uint8_t square = copy.popBitboard(rookBitboardIndex);
        }
    }
    // it is the black moveGen
    else
    {
        while (copy.bitboard[rookBitboardIndex])
        {
            uint8_t square = copy.popBitboard(rookBitboardIndex);
        }
    }
}

/**
 * Generates all the queen moves
 *
 * NOTE: it's just mashed together rook and bishop
 */
void queenGen(Board &board, MoveList &list)
{
}

MoveList &moveGen(Board &board)
{
    MoveList list;

    castleGen(board, list);
    queenGen(board, list);
    rookGen(board, list);
    bishopGen(board, list);
    knightGen(board, list);
    pawnGen(board, list);
    kingGen(board, list);

    return list;
}

MoveList &captureMoveGen(Board &board)
{
    MoveList list;
}