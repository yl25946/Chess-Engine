#pragma once

#include "move.h"
#include "board.h"
#include "moveList.h"

/**
 * Generates pseudo-legal moves (does not consider pins, checks, or castling rights)
 */
MoveList &moveGen(Board &board);

/**
 * Generates pseudo-legal moves (does not consider pins, checks, or castling rights)
 */
MoveList &captureMoveGen(Board &board);