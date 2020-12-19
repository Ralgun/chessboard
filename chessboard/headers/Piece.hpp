#pragma once
#include "Position.hpp"
#include "Move.hpp"
#include <vector>

class Piece
{
private:
    /* data */
public:
    virtual std::vector<Move> findLegalMoves(char x, char y, bool isWhite, Position pos) = 0;
};
