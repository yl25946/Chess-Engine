#include <climits>
#include <stdint.h>

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
    // first 6 bits from the left  is from, next 6 bits from the left is to, last 4 is special moves
    uint16_t move;
    // 0 square is a0

    // cpp hates me so I added this
    Move(){};

    Move(uint8_t from, uint8_t to, MoveFlag specialMoveCode)
    {
        move = USHRT_MAX;
        move &= from << 10;
        move &= to << 4;
        move &= specialMoveCode;
    }

    uint8_t from()
    {
        return (move >> 10);
    }

    uint8_t to()
    {
        return ((move >> 4) & 63);
    }

    uint8_t flag()
    {
        return (move & 15);
    }
};