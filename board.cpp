// #include <string>

// #include "move.h"
#include "board.h"

/** Uses PeSTO's labeling scheme */
std::array<char, 12> numToPiece = {'P', 'p', 'N', 'n', 'B', 'b', 'R', 'r', 'Q', 'q', 'K', 'k'};

// const uint8_t bitboardSize = 12;

class Board
{
public:
    /**
     * 0: white pawn
     * 1: black pawn
     * 2: white knight
     * 3: black knight
     * 4: white bishop
     * 5: black bishop
     * 6: white rook
     * 7: black rook
     * 8: white queen
     * 9: black queen
     * 10: white king
     * 11: black king
     *
     * rigthmost bit is a0
     */
    std::array<uint64_t, 12> bitboard;

    /**
     * from left to right:
     * whose move it is (0 for white, 1 for black)
     * can en passant (current move)
     * can white King Castle
     * can white queen castle
     * can black king castle
     * can black queen castle
     * and rest of the 10 bits counts the pawn move
     */

    /**
     * Counts the number of full moves, increment every time black moves
     */
    uint16_t moveCounter;

    /**
     * Counts the number of half moves elapsed after the last capture, draw a 100
     */
    uint8_t halfMoveCounter;

    /**
     * square where en-passant can happen, all 1s if otherwise
     */
    uint8_t enPassant;

    /**
     * leftmost bit represents who's move it is (0 if white, 1 if black)
     * right 4 bits represent castling rights (1 if true, 0 if false)
     * going from right to left it is white king, white queen, black king, black queen castling rights
     * qkQK
     * 0000
     */
    uint8_t moveAndCastle;

    /**
     * sets up an initial chessboard
     */
    Board()
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
        moveAndCastle = 15;
    }

    /**
     * sets up an initial chessboard
     */
    // Board deepCopy(Board board)
    // {
    //     // copies over the other bitboard
    //     for (int i = 0; i < 12; ++i)
    //         this->bitboard[i] = board.bitboard[i];
    // }

    // Board(std::string FEN)
    // {

    // }

    /**
     *
     */
    bool canWhiteKingCastle()
    {
        return moveAndCastle & 1;
    }

    bool canWhiteKingCastle()
    {
        return moveAndCastle & 1;
    }

    /**
     * Returns the fen representation of the board
     *
     * @returns string fen representation
     */
    std::string fen()
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
        for (int i = 0; i < bitboard.size(); ++i)
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

    // bool
};

// int main()
// {
//     Board b;
//     std::cout << b.fen();
//     return 0;
// }