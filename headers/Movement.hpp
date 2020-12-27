#pragma once

#include "Move.hpp"
#include "Position.hpp"
#include <vector>
#include "Game.hpp"

class Movement
{
private:
    std::vector<std::pair<char, char>> bishopDirections;
    std::vector<std::pair<char, char>> rookDirections;
    std::vector<std::pair<char, char>> queenDirections;
public:
    bool isMoveLegal(Move m, Position pos);
    bool isInCheck(char x, char y, bool isWhiteOnMove, Position pos);
    std::vector<Move> findMovesForPawn(char x, char y, bool isWhite, Position pos);
    std::vector<Move> generateCastlingMoves(bool isWhite, Position pos);
    std::vector<Move> checkKingMovement(char x, char y, bool isWhite, Position pos, void (*f)(Move* pMove));
    std::vector<Move> findLegalMoves(char x, char y, Position pos);
    std::vector<Move> checkKnightMovement(char x, char y, bool isWhite, Position pos, void (*f)(Move* pMove));
    std::vector<Move> checkSlidingMovement(char x, char y, std::vector<std::pair<char, char>> directions, bool isWhite, Position pos, void (*f)(Move* pMove));
    Movement();
};