#pragma once

#include <climits>
#include <stdint.h>
#include <cstdint>

enum MoveFlag : uint8_t
{
    QUIET_MOVE = 0,
    DOUBLE_PAWN_PUSH = 1,
    KINGSIDE_CASTLE = 2,
    QUEENSIDE_CASTLE = 3,
    CAPTURE = 4,
    EN_PASSANT_CAPTURE = 5,
    KNIGHT_PROMOTION = 8,
    BISHOP_PROMOTION = 9,
    ROOK_PROMOTION = 10,
    QUEEN_PROMOTION = 11,
    KNIGHT_PROMOTION_CAPTURE = 12,
    BISHOP_PROMOTION_CAPTURE = 13,
    ROOK_PROMOTION_CAPTURE = 14,
    QUEEN_PROMOTION_CAPTURE = 15,
};

class Move
{
public:
    uint16_t move;
    Move();
    Move(uint8_t from, uint8_t to, MoveFlag specialMoveCode);
    uint8_t from() const;
    uint8_t to() const;
    uint8_t flag() const;
};
