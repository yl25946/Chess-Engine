#include <iostream>
#include <vector>

#include "moveGen.h"

// the board is going to be represented using 12 64 bit unsigned ints, with each bit representing a square
// making memory storage much more efficient
// everything is going to be from white's point of view

// stores the board state after every move.
std::vector<unsigned long long> boardState;

int main()
{
    unsigned long long test[10];
    moveGen(test);
    return 0;
}