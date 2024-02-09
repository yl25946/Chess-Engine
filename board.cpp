#include "board.h"

#define NORTH 8
#define SOUTH -8
#define EAST -1
#define WEST 1

/** Uses PeSTO's labeling scheme */
std::array<char, 12> numToPiece = {'P', 'p', 'N', 'n', 'B', 'b', 'R', 'r', 'Q', 'q', 'K', 'k'};

/**
 * sets up an initial chessboard
 */
Board::Board()
{
    // don't touch this shit, I already set it up
    bitboard[0] = (uint64_t(255)) << 8;
    bitboard[1] = (uint64_t(255)) << 48;
    bitboard[2] = 66;
    bitboard[3] = bitboard[2] << 56;
    bitboard[4] = 36;
    bitboard[5] = bitboard[4] << 56;
    bitboard[6] = 129;
    bitboard[7] = bitboard[6] << 56;
    bitboard[8] = 8;
    bitboard[9] = bitboard[8] << 56;
    bitboard[10] = 16;
    bitboard[11] = bitboard[10] << 56;

    // // default evaluation, don't touch this!!!!!!!!!
    // flags = 15 << 10;

    // sets the different flags
    moveCounter = 0;
    halfMoveCounter = 0;
    // no pawn move yet
    enPassant = UINT8_MAX;
    // everyone can castle, white move
    moveAndCastle = (1 << 7) + 15;
}

void Board::updateBitboard()
{
    for (int i = 0; i < 12; i += 2)
        bitboard[12] |= bitboard[i];

    for (int i = 1; i < 12; i += 2)
        bitboard[13] |= bitboard[i];
}

/**
 *
 */
bool Board::canWhiteKingCastle() const
{
    return moveAndCastle & 1;
}

bool Board::canWhiteQueenCastle() const
{
    return moveAndCastle & 2;
}

bool Board::canBlackKingCastle() const
{
    return moveAndCastle & 4;
}

bool Board::canBlackQueenCastle() const
{
    return moveAndCastle & 8;
}

bool Board::isInCheck() const
{
    return false;
}

uint8_t Board::enPassantSquare() const
{
    return enPassantSquare();
}

/** Use this to check for captures*/
bool Board::isWhitePieceAt(uint8_t square) const
{
    return bitboard[12] & (1 << square);
}

/** Use this to check for captures*/
bool Board::isBlackPieceAt(uint8_t square) const
{
    return bitboard[13] & (1 << square);
}

/**
 * Use this for checking for blocked squares
 */
bool Board::isPieceAt(uint8_t square) const
{

    return isWhitePieceAt(square) && isBlackPieceAt(square);
}

/**
 * returns UINT8_MAX if not found, otherwise returns square
 *
 * MORE EXPENSIVE TO USE THAN isPieceAt, use that if you don't need info on what piece we're doing, but
 */
uint8_t Board::pieceAt(uint8_t square) const
{
    for (int i = 0; i < 12; ++i)
    {
        if (bitboard[i] & (1 << square))
            return i;
    }

    return UINT8_MAX;
}
void Board::push(Move m)
{
    if (!isWhiteMove())
        ++moveCounter;

    uint64_t from = 1 << m.from();
    uint64_t to = 1 << m.to();

    // swaps it from white to black or black to white move
    moveAndCastle ^= (1 << 7);
}

bool Board::isWhiteMove() const
{
    return moveAndCastle & (1 << 7);
}
/**
 * Returns the fen representation of the board
 *
 * @returns string fen representation
 */
std::string Board::fen() const
{
    // default square value is '0'
    std::array<std::array<char, 8>, 8> board;

    // sets all the values to the default value
    for (int i = 0; i < board.size(); ++i)
    {
        for (int j = 0; j < board[0].size(); ++j)
        {
            board[i][j] = '0';
        }
    }

    // throws everything into the board
    for (int i = 0; i < 12; ++i)
    {
        for (int j = 0; j < 64; ++j)
        {
            // if there is a piece at the location
            if (bitboard[i] & (uint64_t(1) << j))
            {
                // when we insert it, flip the board so it is in the correct fen notation
                int index = j ^ 56;
                board[index / 8][index % 8] = numToPiece[i];
            }
        }
    }

    // convert to usable format
    std::string fen = "";
    for (int i = 0; i < 8; ++i)
    {
        // std::string line = "";
        uint8_t counter = 0;
        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j] == '0')
                ++counter;
            else
            {
                if (counter != 0)
                    fen += std::to_string(counter);
                fen += board[i][j];
                counter = 0;
            }
        }
        // if there are empty pieces after the last piece in the row, add it in
        if (counter != 0)
            fen += std::to_string(counter);
        // we need to build this backwards and add the divider "/"
        fen += "/";
    }

    // need to prune the last "/" away
    fen.pop_back();

    return fen;
}

// int main()
// {
//     Board b;
//     std::cout << b.fen();
//     return 0;
// }