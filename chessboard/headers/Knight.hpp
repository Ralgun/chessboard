#include "Piece.hpp"

class Knight : public Piece
{
public:
    std::vector<Move> findLegalMoves(char x, char y, bool isWhite, Position pos);
};