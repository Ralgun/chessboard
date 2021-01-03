#include "Position.hpp"

Position::Position()
{
}

bool Position::operator==(const Position& rhs)
{
    if (isWhiteOnMove != rhs.isWhiteOnMove)
    {
        return false;
    }

    //En passant
    if (lastMove.enpassant || rhs.lastMove.enpassant)
    {
        return false;
    }

    //Castling
    if (whiteLongCastle != rhs.whiteLongCastle || whiteShortCastle != rhs.whiteShortCastle || blackLongCastle != rhs.blackLongCastle || blackShortCastle != rhs.blackShortCastle)
    {
        return false;
    }

    //Board
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            PieceReference ref0 = position[i][j];
            PieceReference ref1 = rhs.position[i][j];
            if (!(ref0.piece == PieceEnum::NOTHING && ref1.piece == PieceEnum::NOTHING) && (ref0.piece != ref1.piece || ref0.isWhite != ref1.isWhite))
            {
                return false;
            }
        }
    }

    return true;
}
