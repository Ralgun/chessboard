#include "Knight.hpp"

std::vector<Move> Knight::findLegalMoves(char x, char y, bool isWhite, Position pos)
{
    std::vector<Move> moves;

    char longDirections[]  = {2, -2};    
    char shortDirections[] = {1, -1};

    for (char longerDir : longDirections)
    {
        for (char shorterDir : shortDirections)
        {
            char xNew = x + longerDir;
            char yNew = y + shorterDir;
            PieceReference p = pos.position[xNew][yNew];
            if ((xNew < 8 && xNew >= 0) && (yNew < 8 && yNew >= 0) && 
                (p.piece == PieceEnum::NOTHING || (p.isWhite != isWhite)))
            {
                moves.push_back({x, y, xNew, yNew});
            }
            xNew = x + shorterDir;
            yNew = y + longerDir;
            p = pos.position[xNew][yNew];
            if ((xNew < 8 && xNew >= 0) && (yNew < 8 && yNew >= 0) && 
                (p.piece == PieceEnum::NOTHING || (p.isWhite != isWhite)))
            {
                moves.push_back({x, y, xNew, yNew});
            }
        }
    }

    return moves;
}