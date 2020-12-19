#include "King.hpp"

std::vector<Move> King::findLegalMoves(char x, char y, bool isWhite, Position pos)
{
    std::vector<Move> moves;

    char a[] = {1, 0, -1};
    for (char xD : a)
    {
        for (char yD : a)
        {
            if ((x+xD >= 0 && x+xD < 8 && y+yD >= 0 && y+yD < 8) && 
                ((pos.position[x+xD][y+yD].piece == PieceEnum::NOTHING) || (pos.position[x+xD][y+yD].isWhite != isWhite)))
            {
                moves.push_back({x, y, x+xD, y+yD});
            }
        }
    }

    return moves;    
}