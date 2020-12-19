#include "Rook.hpp"

std::vector<Move> Rook::findLegalMoves(char x, char y, bool isWhite, Position pos)
{
    std::vector<Move> moves;

    //TODO probably not worth it creating 4 different cycles to avoid few if statements...
    //bottom
    for (char i = y; i >= 0; i--)
    {
        if (pos.position[x][i].piece == PieceEnum::NOTHING)
        {
            moves.push_back({x, y, x, i});
        }
        else if (pos.position[x][i].isWhite == isWhite)
        {
            break;
        }
        else
        {
            moves.push_back({x, y, x, i});
            break;
        }
        
    }
    //left
    for (char i = x; i >= 0; i--)
    {
        if (pos.position[i][y].piece == PieceEnum::NOTHING)
        {
            moves.push_back({x, y, i, y});

        }
        else if (pos.position[i][y].isWhite == isWhite)
        {
            break;
        }
        else
        {
            moves.push_back({x, y, i, y});
            break;
        }
    }
    //top
    for (char i = y+1; i < 8; i++)
    {
        if (pos.position[x][i].piece == PieceEnum::NOTHING)
        {
            moves.push_back({x, y, x, i});

        }
        else if (pos.position[x][i].isWhite == isWhite)
        {
            break;
        }
        else
        {
            moves.push_back({x, y, x, i});
            break;
        }
    }
    //right
    for (char i = x+1; i < 8; i++)
    {
        if (pos.position[i][y].piece == PieceEnum::NOTHING)
        {
            moves.push_back({x, y, i, y});

        }
        else if (pos.position[i][y].isWhite == isWhite)
        {
            break;
        }
        else
        {
            moves.push_back({x, y, i, y});
            break;
        }
    }

    return moves;
}