#include "Position.hpp"
#include "Move.hpp"
#include "Piece.hpp"
#include "Pawn.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include "King.hpp"
#include "PieceReference.hpp"
#include <string>
#include <vector>
#include <utility>

class Game
{
private:
    std::vector<Position> positions;
    std::vector<Piece*> pieceSystems;
    std::pair<char, char> convertRaw(char a, char b); 
    PieceEnum symbolToEnum(char symbol);
    //Symbols used to conver between PieceEnum and symbol
    char* symbols = "prnbqk";
    char enumToSymbol(PieceEnum e);
    void setupPosition(PieceReference position[8][8]);
public:
    std::string getStringPosition();
    bool rawMove(std::string move);
    bool move(Move move);
    std::vector<Move> findMovesForAllPieces();
    Game();
    ~Game();
};
