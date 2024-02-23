#pragma once

#include "move.h"
#include "board.h"
#include "moveList.h"

/**
 * Generates pseudo-legal moves (does not consider pins, checks, or castling rights, or moving into check)
 */
MoveList &moveGen(Board &board);

/**
 * Generates pseudo-legal moves (only need to consider checks & pins?)
 */
MoveList &captureMoveGen(Board &board);