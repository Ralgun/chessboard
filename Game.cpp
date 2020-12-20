#include "Game.hpp"
#include <iostream>

char alphabetToNumber(char a)
{
    return a - 97;
}

PieceEnum Game::symbolToEnum(char a)
{
    a = tolower(a);

    switch (a)
    {
        case 'p':
            return PieceEnum::PAWN;
        case 'r':
            return PieceEnum::ROOK;
        case 'n':
            return PieceEnum::KNIGHT;
        case 'b':
            return PieceEnum::BISHOP;
        case 'q':
            return PieceEnum::QUEEN;
        case 'k':
            return PieceEnum::KING;
        default:
            return PieceEnum::NOTHING;
    }
}

char Game::enumToSymbol(PieceEnum e)
{
    char a;
    int enumValue = e;
    if (enumValue == -1)
    {
        return '.';
    }
    a = symbols[enumValue];

    return a;
}

//Puts all piece on standard position
void Game::setupPosition(PieceReference pieces[8][8])
{

    //Make blank position
    for (int i = 2; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            pieces[j][i] = {PieceEnum::NOTHING, false};
        }
    }

    //Add pawns
    for (int i = 0; i < 8; i++)
    {
        pieces[i][1] = {PieceEnum::PAWN, true};
        pieces[i][6] = {PieceEnum::PAWN, false};
    }

    //Add ohter pieces
    //Rooks
    pieces[0][0] = {PieceEnum::ROOK, true};
    pieces[7][0] = {PieceEnum::ROOK, true};
    pieces[0][7] = {PieceEnum::ROOK, false};
    pieces[7][7] = {PieceEnum::ROOK, false};
    //Knights
    pieces[1][0] = {PieceEnum::KNIGHT, true};
    pieces[6][0] = {PieceEnum::KNIGHT, true};
    pieces[1][7] = {PieceEnum::KNIGHT, false};
    pieces[6][7] = {PieceEnum::KNIGHT, false};
    //Bishops
    pieces[2][0] = {PieceEnum::BISHOP, true};
    pieces[5][0] = {PieceEnum::BISHOP, true};
    pieces[2][7] = {PieceEnum::BISHOP, false};
    pieces[5][7] = {PieceEnum::BISHOP, false};
    //Queens
    pieces[3][0] = {PieceEnum::QUEEN, true};
    pieces[3][7] = {PieceEnum::QUEEN, false};
    //Kings
    pieces[4][0] = {PieceEnum::KING, true};
    pieces[4][7] = {PieceEnum::KING, false};
    //Board's complete
}

Game::Game() 
{
    Position pos;
    setupPosition(pos.position);

    pos.firstMove     = true;
    pos.isWhiteOnMove = true;
    

    positions.push_back(pos);

    //Setup piece systems
    pieceSystems.push_back(new Pawn());
    pieceSystems.push_back(new Rook());
    pieceSystems.push_back(new Knight());
    pieceSystems.push_back(new Bishop());
    pieceSystems.push_back(new Queen());
    pieceSystems.push_back(new King());

}

bool Game::rawMove(std::string raw)
{
    Position pos = positions.back();
    //Variables before conversion
    char xStart = raw[0];
    //Converting character to digit
    char yStart = raw[1] - '0';
    char xEnd   = raw[2];
    //Converting character to digit
    char yEnd   = raw[3] - '0';

    //Conversion
    Move moveVar;
    moveVar.xStart  = alphabetToNumber(xStart);
    moveVar.yStart  = yStart - 1;
    moveVar.xEnd    = alphabetToNumber(xEnd);
    moveVar.yEnd    = yEnd - 1; 
    //After conversion
    if (raw.length() == 5)
    {
        //Promotion
        moveVar.promotion = symbolToEnum(raw[4]);
    }

    //Verify
    PieceReference p = pos.position[moveVar.xStart][moveVar.yStart];

    if (p.isWhite != pos.isWhiteOnMove)
    {
        return true;
    }

    std::vector<Move> pieceMoves = pieceSystems[p.piece]->findLegalMoves(moveVar.xStart, moveVar.yStart, p.isWhite, pos);
    for (Move m : pieceMoves)
    {
        if (m == moveVar)
        {
            return move(m);
        }
    }
    return true;
}

bool Game::move(Move pMove)
{

    //TODO castles and en peasant
    //Fetching current position
    Position pos     = positions.back();
    PieceReference p = pos.position[pMove.xStart][pMove.yStart];
    //Promotion
    if (pMove.promotion != PieceEnum::NOTHING)
    {
        p.piece = pMove.promotion;
    }

    pos.position[pMove.xStart][pMove.yStart].piece = PieceEnum::NOTHING;
    //Special case: en passant
    if (pMove.enpassant)
    {
        char mult = pos.isWhiteOnMove ? -1 : 1;
        pos.position[pMove.xEnd][pMove.yEnd+mult].piece = PieceEnum::NOTHING;
    }
    pos.position[pMove.xEnd][pMove.yEnd] = p;

    pos.isWhiteOnMove = !pos.isWhiteOnMove;
    pMove.moved = p.piece;
    pos.lastMove = pMove;
    pos.firstMove = false;

    positions.push_back(pos);

    return false;
}

std::string Game::getStringPosition()
{
    std::string s = "";
    Position pos  = positions.back();

    for (char y = 7; y >= 0; y--)
    {
        for (char x = 0; x < 8; x++)
        {
            char a = enumToSymbol(pos.position[x][y].piece);
            if (pos.position[x][y].isWhite)
            {
                a = toupper(a);
            }
            s += a;
        }
        s += "\n";
    }

    return s;
}

Game::~Game()
{}