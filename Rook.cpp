#include "Rook.hpp"

std::vector<Move> Rook::findLegalMoves(char x, char y, bool isWhite, Position pos)
{
    std::vector<Move> moves;

    //TODO This is just terrible.
    //bottom
    for (char i = y-1; i >= 0; i--)
    {
        if (pos.position[x][i].piece == PieceEnum::NOTHING)
        {
            Move m = {x, y, x, i};
            if (x == 7)
            {
                m.removeShortCastle = true;
            }
            else if (x == 0)
            {
                m.removeLongCastle = true;
            }
            moves.push_back(m);
        }
        else if (pos.position[x][i].isWhite == isWhite)
        {
            break;
        }
        else
        {
            Move m = {x, y, x, i};
            if (x == 7)
            {
                m.removeShortCastle = true;
            }
            else if (x == 0)
            {
                m.removeLongCastle = true;
            }
            moves.push_back(m);
            break;
        }
        
    }
    //left
    for (char i = x-1; i >= 0; i--)
    {
        if (pos.position[i][y].piece == PieceEnum::NOTHING)
        {
            Move m = {x, y, i, y};
            if (x == 7)
            {
                m.removeShortCastle = true;
            }
            else if (x == 0)
            {
                m.removeLongCastle = true;
            }
            moves.push_back(m);
        }
        else if (pos.position[i][y].isWhite == isWhite)
        {
            break;
        }
        else
        {
            Move m = {x, y, i, y};
            if (x == 7)
            {
                m.removeShortCastle = true;
            }
            else if (x == 0)
            {
                m.removeLongCastle = true;
            }
            moves.push_back(m);
            break;
        }
    }
    //top
    for (char i = y+1; i < 8; i++)
    {
        if (pos.position[x][i].piece == PieceEnum::NOTHING)
        {
            Move m = {x, y, x, i};
            if (x == 7)
            {
                m.removeShortCastle = true;
            }
            else if (x == 0)
            {
                m.removeLongCastle = true;
            }
            moves.push_back(m);
        }
        else if (pos.position[x][i].isWhite == isWhite)
        {
            break;
        }
        else
        {
            Move m = {x, y, x, i};
            if (x == 7)
            {
                m.removeShortCastle = true;
            }
            else if (x == 0)
            {
                m.removeLongCastle = true;
            }
            moves.push_back(m);
            break;
        }
    }
    //right
    for (char i = x+1; i < 8; i++)
    {
        if (pos.position[i][y].piece == PieceEnum::NOTHING)
        {
            Move m = {x, y, i, y};
            if (x == 7)
            {
                m.removeShortCastle = true;
            }
            else if (x == 0)
            {
                m.removeLongCastle = true;
            }
            moves.push_back(m);
        }
        else if (pos.position[i][y].isWhite == isWhite)
        {
            break;
        }
        else
        {
            Move m = {x, y, i, y};
            if (x == 7)
            {
                m.removeShortCastle = true;
            }
            else if (x == 0)
            {
                m.removeLongCastle = true;
            }
            moves.push_back(m);
            break;
        }
    }

    return moves;
}