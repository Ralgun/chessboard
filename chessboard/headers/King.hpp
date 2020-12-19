#include "Piece.hpp"

class King : public Piece
{
public:
    std::vector<Move> findLegalMoves(char x, char y, bool isWhite, Position pos);
};