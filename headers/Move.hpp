#pragma once
#include "PieceEnum.hpp"

struct Move 
{
    char xStart, yStart, xEnd, yEnd;
    PieceEnum moved;
    PieceEnum promotion = PieceEnum::NOTHING;
    bool castle = false;
    bool removeShortCastle = false;
    bool removeLongCastle = false;
    bool enpassant = false;
    char oldRookX, oldRookY, newRookX, newRookY;
    bool givesCheck = false;
    bool operator==(const Move& rhs);
};