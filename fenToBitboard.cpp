#include <string>

#include "board.h"

Board fenToBitboard(std::string fen)
{
    for (char c : fen)
    {
        if (c == ' ')
            continue;
    }
}