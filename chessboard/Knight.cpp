#include "Knight.hpp"

std::vector<Move> Knight::findLegalMoves(char x, char y, bool isWhite, Position pos)
{
    std::vector<Move> moves;

    //Top left
    if ((x-1 >= 0 && y+2 < 8) && (pos.position[x-1][y+2].piece == PieceEnum::NOTHING || pos.position[x-1][y+2].isWhite != isWhite))
    {
        moves.push_back({x, y, x-1, y+2});
    }
    //Top right
    if ((x+1 < 8 && y+2 < 8) && (pos.position[x+1][y+2].piece == PieceEnum::NOTHING || pos.position[x+1][y+2].isWhite != isWhite))
    {
        moves.push_back({x, y, x+1, y+2});
    }
    //Right top
    if ((x+2 < 8 && y+1 < 8) && (pos.position[x+2][y+1].piece == PieceEnum::NOTHING || pos.position[x+2][y+1].isWhite != isWhite))
    {
        moves.push_back({x, y, x+2, y+1});
    }
    //Right bottom
    if ((x+2 < 8 && y-1 >= 0) && (pos.position[x+2][y-1].piece == PieceEnum::NOTHING || pos.position[x+2][y-1].isWhite != isWhite))
    {
        moves.push_back({x, y, x+2, y-1});
    }
    //Bottom left
    if ((x-1 >= 0 && y-2 >= 0) && (pos.position[x-1][y-2].piece == PieceEnum::NOTHING || pos.position[x-1][y-2].isWhite != isWhite))
    {
        moves.push_back({x, y, x-1, y-2});
    }
    //Bottom right
    if ((x+1 < 8 && y-2 >= 0) && (pos.position[x+1][y-2].piece == PieceEnum::NOTHING || pos.position[x+1][y-2].isWhite != isWhite))
    {
        moves.push_back({x, y, x+1, y-2});
    }
    //Left bottom
    if ((x-2 >= 0 && y-1 >= 0) && (pos.position[x-2][y-1].piece == PieceEnum::NOTHING || pos.position[x-2][y-1].isWhite != isWhite))
    {
        moves.push_back({x, y, x-2, y-1});
    }
    //Left top
    if ((x-2 >= 0 && y+1 < 8) && (pos.position[x-2][y+1].piece == PieceEnum::NOTHING || pos.position[x-2][y+1].isWhite != isWhite))
    {
        moves.push_back({x, y, x-2, y+1});
    }

    return moves;
}