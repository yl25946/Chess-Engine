#ifndef SPAGHET_ZOBRIST_H
#define SPAGHET_ZOBRIST_H

#include "boardState.h"

// no idea what the fuck this does
class BoardState; // forward declaration

void hash(BoardState &board);

void quickHash(BoardState &currBoard, BoardState &nextBoard);

#endif