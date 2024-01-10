#include <string>

#include "move.h"

class Board
{
public:
    /**
     * 0: white pawn
     * 1: white rook
     * 2: white knight
     * 3: white bishop
     * 4: white queen
     * 5: white king
     * 6: black pawn
     * 7: black rook
     * 8: black knight
     * 9: black bishop
     * 10: black queen
     * 11: black king
     *
     * leftmost bit is topleft square, going side then down
     */
    uint64_t bitboard[12];

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
    bool canEnPassant;
    // oif we can en passant, this is the square of the en-passantable pawn
    uint8_t enPassantPawnMove;
    bool canWhiteKingCastle;
    bool canWhiteQueenCastle;
    bool canBlackKingCastle;
    bool canBlackQueenCastle;
    bool whiteMove;

    /**
     * Counts half moves until stalemate
     */
    uint8_t staleMateCount;
    /**
     * Counts the number of full moves, increment every time black moves
     */
    uint16_t moveCounter;

    /**
     * Shows the previous move
     * first 8 bits from the left stores the from, next 8 bit stores the to
     * squares are counted from white's perspective, where top left is one, and go right and then go down
     */
    uint16_t previousMove;

    /**
     * sets up an initial chessboard
     */
    Board()
    {
        // don't touch this shit
        bitboard[0] = 65280;
        bitboard[1] = 129;
        bitboard[2] = 66;
        bitboard[3] = 36;
        bitboard[4] = 16;
        bitboard[5] = 8;
        bitboard[6] = 255 << 48;
        bitboard[7] = bitboard[1] << 56;
        bitboard[8] = bitboard[2] << 56;
        bitboard[9] = bitboard[3] << 56;
        bitboard[10] = bitboard[4] << 56;
        bitboard[11] = bitboard[5] << 56;

        // // default evaluation, don't touch this!!!!!!!!!
        // flags = 15 << 10;

        // default board state
        canEnPassant = false;
        canWhiteKingCastle = true;
        canWhiteQueenCastle = true;
        canBlackKingCastle = true;
        canBlackQueenCastle = true;
        whiteMove = true;

        // sets the move counters
        moveCounter = 0;
        staleMateCount = 0;
    }

    /**
     * sets up an initial chessboard
     */
    Board(uint64_t bitboard[], bool whiteMove, bool canEnPassant, uint8_t enPassantPawnMove, bool canWhiteKingCastle, bool canWhiteQueenCastle, bool canBlackKingCastle, bool canBlackQueenCastle)
    {
        // copies over the other bitboard
        for (int i = 0; i < 8; ++i)
            this->bitboard[i] = bitboard[i];

        this->whiteMove = whiteMove;
        this->canEnPassant = canEnPassant;
        this->enPassantPawnMove = enPassantPawnMove;
        this->canWhiteKingCastle = canWhiteKingCastle;
        this->canWhiteQueenCastle = canWhiteQueenCastle;
        this->enPassantPawnMove = enPassantPawnMove;
    }

    Board(std::string FEN)
    {
    }

    bool isValidBoardState(bool whiteMove)
    {
        if (whiteMove)
        {
            uint64_t kingSquare = bitboard[5];
            // pawn
            for (int i = 0; i < 64; ++i)
            {
            }
            // rook

            // knight

            // bishop

            // queen

            // king
        }
        else
        {
            uint64_t kingSquare = bitboard[11];
            // pawn
            for (int i = 0; i < 64; ++i)
            {
            }
            // rook

            // knight

            // bishop

            // queen

            // king
        }
    }

    std::string toFEN()
    {
        char board[8][8];
        // white pawn
        }
};