#include <vector>
#include <string>
#include <array>
#include <climits>
#include <stdint.h>

#include "move.h"

class Board
{
public:
    std::array<uint64_t, 12> bitboard;
    uint16_t moveCounter;
    uint8_t halfMoveCounter;
    uint8_t enPassant;
    uint8_t moveAndCastle;
    Board();
    Board &deepCopy();
    bool canWhiteKingCastle();
    bool canWhiteQueenCastle();
    bool canBlackKingCastle();
    bool canBlackQueenCastle();
    bool isInCheck();
    uint8_t pieceAt(uint8_t square);
    void push(Move m);
    bool isWhiteMove();
    std::string fen();
};