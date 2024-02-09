#include <array>

#include "move.h"

class MoveList
{
public:
    std::array<Move, 255> oversizedArray;
    uint8_t size;
    MoveList();
    inline void insert(Move m);
};