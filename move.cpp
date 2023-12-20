#include <climits>
#include <stdint.h>

class Move
{
public:
    // first 6 bits from the left  is from, next 6 bits from the left is to, last 4 is special moves
    uint16_t move;
    // from is an int from 0 - 63, 0 is the top left corner of the board (oriented white) and left then bottom
    Move(uint8_t from, uint8_t to, uint8_t specialMoveCode)
    {
        move = USHRT_MAX;
        move &= from << 10;
        move &= to << 4;
        move &= specialMoveCode;
    }
};