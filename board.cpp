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
    bool canEnPassant;
    bool canWhiteKingCastle;
    bool canWhiteQueenCastle;
    bool canBlackKingCastle;
    bool canBlackQueenCastle;
    uint8_t twoPawnMove;
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
        canEnPassant = false;
        canWhiteKingCastle = true;
        canBlackKingCastle = true;
        canWhiteQueenCastle = true;
        canBlackQueenCastle = true;
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
};