#pragma once

#include <vector>
#include <string>
#include <array>
#include <climits>
#include <stdint.h>

#include "move.h"
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
     * 12: all the white pieces
     * 13: all the black pieces
     *
     * rightmost bit is a0
     */
    std::array<uint64_t, 14> bitboard;

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
    Board();
    void updateBitboard();
    // Board &deepCopy() const;
    bool canWhiteKingCastle() const;
    bool canWhiteQueenCastle() const;
    bool canBlackKingCastle() const;
    bool canBlackQueenCastle() const;
    bool isInCheck() const;
    uint8_t enPassantSquare() const;
    bool isBlackPieceAt(uint8_t square) const;
    bool isWhitePieceAt(uint8_t square) const;
    bool isPieceAt(uint8_t square) const;
    uint8_t pieceAt(uint8_t square) const;
    void push(Move m);
    bool isWhiteMove() const;
    std::string fen() const;
};
