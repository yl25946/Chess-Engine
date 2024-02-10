#include "moveList.h"

MoveList::MoveList()
{
}

void MoveList::insert(Move m)
{
    oversizedArray[size++] = m;
}
