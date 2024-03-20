#pragma once

#include "move.h"
#include "boardState.h"
#include "moveList.h"

/**
 * Generates pseudo-legal moves (does not consider pins, checks, or castling rights, or moving into check)
 */
void moveGen(BoardState &board, MoveList &m);

/**
 * Generates pseudo-legal moves (only need to consider checks & pins?)
 */
void captureMoveGen(BoardState &board, MoveList &m);

/**
 * Generates the magic bitboards for sliding pieces
 */
void generateMagicBitboards();