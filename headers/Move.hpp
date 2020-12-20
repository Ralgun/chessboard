#pragma once
#include "PieceEnum.hpp"

struct Move 
{
    char xStart, yStart, xEnd, yEnd;
    PieceEnum moved;
    PieceEnum promotion = PieceEnum::NOTHING;
    bool castle = false;
    bool enpassant = false;
    char oldRookX, oldRookY, newRookX, newRookY;
    bool operator==(const Move& rhs);
};