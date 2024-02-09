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
     * rightmost bit is a0
     */
    std::array<uint64_t, 12> bitboard;

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
     * leftmost bit represents who's move it is (1 if white, 0 if black)
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
        moveAndCastle = (1 << 15) + 15;
    }

    /**
     * sets up an initial chessboard
     */
    Board &deepCopy()
    {
        Board board;
        // copies over the other bitboard
        for (int i = 0; i < 12; ++i)
            this->bitboard[i] = board.bitboard[i];

        board.moveCounter = this->moveCounter;
        board.halfMoveCounter = this->halfMoveCounter;
        board.moveAndCastle = this->moveAndCastle;
        board.enPassant = this->enPassant;

        return board;
    }

    /**
     *
     */
    bool canWhiteKingCastle()
    {
        return moveAndCastle & 1;
    }

    bool canWhiteQueenCastle()
    {
        return moveAndCastle & 2;
    }

    bool canBlackKingCastle()
    {
        return moveAndCastle & 4;
    }

    bool canBlackQueenCastle()
    {
        return moveAndCastle & 8;
    }

    bool isInCheck()
    {
        return false;
    }

    /**
     * returns UINT8_MAX if not found, otherwise returns square
     */
    uint8_t pieceAt(uint8_t square)
    {
        for (uint8_t i = 0; i < bitboard.size(); ++i)
            if (bitboard[i] & (1 << square))
                return i;

        return UINT8_MAX;
    }

    void push(Move m)
    {
        if (!isWhiteMove())
            ++moveCounter;

        uint64_t from = 1 << m.from();
        uint64_t to = 1 << m.to();

        // swaps it from white to black or black to white move
        moveAndCastle ^= (1 << 7);
    }

    bool isWhiteMove()
    {
        return moveAndCastle & (1 << 7);
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
};

// int main()
// {
//     Board b;
//     std::cout << b.fen();
//     return 0;
// }