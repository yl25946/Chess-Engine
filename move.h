#include <climits>
#include <stdint.h>

class Move
{
public:
    uint16_t move;
    Move(uint8_t from, uint8_t to, uint8_t specialMoveCode);
};