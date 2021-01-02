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
    std::vector<Move> currentMoves;
    std::vector<Position> positions;
    std::pair<char, char> convertRaw(char a, char b); 
    PieceEnum symbolToEnum(char symbol);
    //Symbols used to conver between PieceEnum and symbol
    char* symbols = "prnbqk";
    char enumToSymbol(PieceEnum e);
    Position setupPosition();
public:
    std::string getStringPosition();
    bool rawMove(std::string move);
    bool makeMoveToGame(Move m);
    std::vector<Move> assingNumOfMoves(Position &pos);
    std::vector<Move> findMovesForAllPieces();
    void isGameDone(Position &pos);
    Game();
    ~Game();
};
