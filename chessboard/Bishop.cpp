#include "Bishop.hpp"

std::vector<Move> Bishop::findLegalMoves(char x, char y, bool isWhite, Position pos)
{
    std::vector<Move> moves;

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