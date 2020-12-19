#pragma once
#include "PieceReference.hpp"
#include "Move.hpp"

struct Position
{
    PieceReference position[8][8];
    //TODO Group booleans
    bool whiteShortCastle;
    bool whiteLongCastle;
    bool blackShortCastle;
    bool blackLongCastle;
    bool firstMove = false;
    Move lastMove;
    bool isWhiteOnMove = true;
    Position();
      
};

