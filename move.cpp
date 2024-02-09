#include "move.h"

Move::Move(){};

Move::Move(uint8_t from, uint8_t to, MoveFlag specialMoveCode)
{
    move = 0;
    // you need or to maintain the values later
    move |= from << 10;
    move |= to << 4;
    move |= specialMoveCode;
}

// if anything breaks remove const
uint8_t Move::from() const
{
    return (move >> 10);
}

uint8_t Move::to() const
{
    return ((move >> 4) & 63);
}

uint8_t Move::flag() const
{
    return (move & 15);
}
