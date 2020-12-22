#include "Queen.hpp"

std::vector<Move> Queen::findLegalMoves(char x, char y, bool isWhite, Position pos)
{
    std::vector<Move> moves;

    //Rook
    for (char i = y-1; i >= 0; i--)
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
    for (char i = x-1; i >= 0; i--)
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
    //Bishop
    char a[] = {-1, 1};
    for (char xD : a)
    {
        for (char yD : a)
        {
            for (int i = 1; i < 8; i++)
            {
                char xNew = x + xD*i;
                char yNew = y + yD*i;

                if (pos.position[xNew][yNew].piece == PieceEnum::NOTHING)
                {
                    moves.push_back({x, y, xNew, yNew});

                }
                else if (pos.position[xNew][yNew].isWhite == isWhite)
                {
                    break;
                }
                else
                {
                    moves.push_back({x, y, xNew, yNew});
                    break;
                }
            }
        }
    }

    return moves;
}