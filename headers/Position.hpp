#pragma once
#include "PieceReference.hpp"
#include "Move.hpp"
#include "GameStateEnum.hpp"

struct Position
{
    PieceReference position[8][8];
    //TODO Group booleans
    bool whiteShortCastle = true;
    bool whiteLongCastle = true;
    bool blackShortCastle = true;
    bool blackLongCastle = true;
    bool firstMove = false;
    Move lastMove;
    bool isWhiteOnMove = true;
    bool inCheck = false;
    char xWhiteKing, yWhiteKing, xBlackKing, yBlackKing;
    char numOfMovesAvailable;
    GameState state = GameState::PLAYING;
    Position();
    bool operator==(const Position& rhs);
      
};