#include "PieceReference.hpp"

PieceReference::PieceReference()
{

}

PieceReference::PieceReference(PieceEnum pPiece, bool pIsWhite)
{
    piece   = pPiece;
    isWhite = pIsWhite;
}