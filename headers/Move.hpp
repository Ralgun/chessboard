#pragma once
#include "PieceEnum.hpp"

struct Move 
{
    char xStart, yStart, xEnd, yEnd;
    PieceEnum promotion = PieceEnum::NOTHING;
    bool castle = false;
    PieceEnum moved;
    char oldRookX, oldRookY, newRookX, newRookY;
    bool operator==(const Move& rhs);
};