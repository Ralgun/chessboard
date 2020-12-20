#include "Pawn.hpp"
#include <iostream>

std::vector<Move> Pawn::findLegalMoves(char x, char y, bool isWhite, Position pos)
{
    std::vector<Move> moves;
    char mult     = 1;
    char initialY = 1;
    char enemyY   = 6;
    if (!isWhite)
    {
        mult     = -1;
        initialY =  6;
        enemyY   =  1;
    }
    
    if (pos.position[x][y+mult].piece == PieceEnum::NOTHING)
    {
        //Promotion
        if (y == enemyY)
        {
            Move m = {x, y, x, y+mult};
            m.promotion = PieceEnum::QUEEN;
            moves.push_back(m);
            m.promotion = PieceEnum::ROOK;
            moves.push_back(m);
            m.promotion = PieceEnum::KNIGHT;
            moves.push_back(m);
            m.promotion = PieceEnum::BISHOP;
            moves.push_back(m);
        }
        else 
        {
            moves.push_back({x, y, x, y+mult});
            if (y == initialY && pos.position[x][y+2*(mult)].piece == PieceEnum::NOTHING)
            {
                moves.push_back({x, y, x, y+(2*mult)});
            }
        }
    }
    //Captures
    if (pos.position[x-1][y+mult].piece != PieceEnum::NOTHING && pos.position[x-1][y+mult].isWhite != isWhite)
    {
        if (y == enemyY)
        {
            Move m = {x, y, x-1, y+mult};
            m.promotion = PieceEnum::QUEEN;
            moves.push_back(m);
            m.promotion = PieceEnum::ROOK;
            moves.push_back(m);
            m.promotion = PieceEnum::KNIGHT;
            moves.push_back(m);
            m.promotion = PieceEnum::BISHOP;
            moves.push_back(m);
        }
        else
        {
            moves.push_back({x, y, x-1, y+mult});
        }
    }
    if (pos.position[x+1][y+mult].piece != PieceEnum::NOTHING && pos.position[x+1][y+mult].isWhite != isWhite)
    {
        if (y == enemyY)
        {
            Move m = {x, y, x+1, y+mult};
            m.promotion = PieceEnum::QUEEN;
            moves.push_back(m);
            m.promotion = PieceEnum::ROOK;
            moves.push_back(m);
            m.promotion = PieceEnum::KNIGHT;
            moves.push_back(m);
            m.promotion = PieceEnum::BISHOP;
            moves.push_back(m);
        }
        else
        {
            moves.push_back({x, y, x+1, y+mult});
        }
    }

    //En passant
    if (pos.lastMove.moved == PieceEnum::PAWN && abs(pos.lastMove.xEnd - x) == 1 &&
        pos.lastMove.yStart == enemyY && y == pos.lastMove.yEnd)
    {
        Move m = {x, y, pos.lastMove.xEnd, y+mult};
        m.enpassant = true;
        moves.push_back(m);
    }

    return moves;
}