#include "boardState.h"
#include <stack>
#include <vector>

class Board
{
public:
    std::stack<BoardState, std::vector<BoardState>> stack;

    Board();
    // returns the top of the stack
    BoardState &current() const;
    // pushes in a new move
    BoardState &push(Move m);
    void push(BoardState boardState);
    void pop();
};