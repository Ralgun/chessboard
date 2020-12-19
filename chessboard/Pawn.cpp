#include "Pawn.hpp"
#include <iostream>

std::vector<Move> Pawn::findLegalMoves(char x, char y, bool isWhite, Position pos)
{
    std::vector<Move> moves;
    char mult     = 1;
    char initialY = 1;
    if (!isWhite)
    {
        std::cout << "black";
        mult     = -1;
        initialY =  6;
    }
    
    if (pos.position[x][y+mult].piece == PieceEnum::NOTHING)
    {
        moves.push_back({x, y, x, y+mult});
        if (y == initialY && pos.position[x][y+2*(mult)].piece == PieceEnum::NOTHING)
        {
            moves.push_back({x, y, x, y+(2*mult)});
        }
    }
    //Captures
    //TODO en peasant
    if (pos.position[x-1][y+mult].piece != PieceEnum::NOTHING && pos.position[x-1][y+mult].isWhite != isWhite)
    {
        moves.push_back({x, y, x-1, y+mult});
    }
    if (pos.position[x+1][y+mult].piece != PieceEnum::NOTHING && pos.position[x+1][y+mult].isWhite != isWhite)
    {
        moves.push_back({x, y, x+1, y+mult});
    }

    return moves;
}