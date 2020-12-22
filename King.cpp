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

    //Castles
    if (isWhite)
    {
        if (pos.whiteShortCastle && pos.position[x+1][0].piece == PieceEnum::NOTHING && pos.position[x+2][0].piece == PieceEnum::NOTHING)
        {
            Move m = {x, y, x+2, y};
            m.castle = true;
            m.removeLongCastle;
            m.removeShortCastle;
            m.oldRookX = 7;
            m.oldRookY = 0;
            m.newRookX = x+1;
            m.newRookY = 0;
            moves.push_back(m);
        }
        if (pos.whiteLongCastle && pos.position[x-1][0].piece == PieceEnum::NOTHING && pos.position[x-2][0].piece == PieceEnum::NOTHING && pos.position[x-3][0].piece == PieceEnum::NOTHING)
        {
            Move m = {x, y, x-2, y};
            m.castle = true;
            m.removeLongCastle;
            m.removeShortCastle;
            m.oldRookX = 0;
            m.oldRookY = 0;
            m.newRookX = x-1;
            m.newRookY = 0;
            moves.push_back(m);
        }
    }
    else if (!isWhite)
    {
        if (pos.blackShortCastle && pos.position[x+1][7].piece == PieceEnum::NOTHING && pos.position[x+2][7].piece == PieceEnum::NOTHING)
        {
            Move m = {x, y, x+2, y};
            m.castle = true;
            m.removeLongCastle;
            m.removeShortCastle;
            m.oldRookX = 7;
            m.oldRookY = 7;
            m.newRookX = x+1;
            m.newRookY = 7;
            moves.push_back(m);
        }
        if (pos.blackLongCastle && pos.position[x-1][7].piece == PieceEnum::NOTHING && pos.position[x-2][7].piece == PieceEnum::NOTHING && pos.position[x-3][7].piece == PieceEnum::NOTHING)
        {
            Move m = {x, y, x-2, y};
            m.castle = true;
            m.removeLongCastle;
            m.removeShortCastle;
            m.oldRookX = 0;
            m.oldRookY = 7;
            m.newRookX = x-1;
            m.newRookY = 7;
            moves.push_back(m);
        }
    }

    return moves;    
}