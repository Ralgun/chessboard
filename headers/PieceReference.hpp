#pragma once
#include "PieceEnum.hpp"

struct PieceReference
{
    PieceEnum piece;
    bool isWhite;
    PieceReference();
    PieceReference(PieceEnum pPiece, bool pIsWhite);
};