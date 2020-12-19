#include "Piece.hpp"

class Pawn : public Piece
{
public:
    std::vector<Move> findLegalMoves(char x, char y, bool isWhite, Position pos);
};