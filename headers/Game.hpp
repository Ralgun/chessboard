#include "Position.hpp"
#include "Move.hpp"
#include "PieceReference.hpp"
#include "Movement.hpp"
#include <string>
#include <vector>
#include <utility>

class Game
{
private:
    Movement movement;
    std::vector<Position> positions;
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
