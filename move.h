#include <climits>
#include <stdint.h>

enum MoveFlag;
class Move
{
public:
    uint16_t move;
    Move();
    Move(uint8_t from, uint8_t to, MoveFlag specialMoveCode);
    uint8_t from();
    uint8_t to();
    uint8_t flag();
};