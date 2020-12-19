#include "Piece.hpp"

class Bishop : public Piece
{
public:
    std::vector<Move> findLegalMoves(char x, char y, bool isWhite, Position pos);
};