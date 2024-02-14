#pragma once

#include "move.h"
#include "board.h"
#include "moveList.h"

MoveList &moveGen(Board &board);

MoveList &captureMoveGen(Board &board);