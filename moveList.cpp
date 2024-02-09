#include "moveList.h"

MoveList::MoveList()
{
}

inline void MoveList::insert(Move m)
{
    oversizedArray[size++] = m;
}
