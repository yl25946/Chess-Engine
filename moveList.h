#ifndef SPAGHET_MOVE_LIST_H
#define SPAGHET_MOVE_LIST_H

#include <array>

#include "move.h"

class MoveList
{
public:
    std::array<Move, 255> oversizedArray;
    uint8_t size = 0;
    MoveList();
    void insert(Move m);
};

#endif