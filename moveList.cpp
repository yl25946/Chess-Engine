#include "moveList.h"

class MoveList
{
public:
    std::array<Move, 255> oversizedArray;
    uint8_t size = 0;

    MoveList()
    {
    }

    inline void insert(Move m)
    {
        oversizedArray[size++];
    }
};