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
Position Game::setupPosition()
{
    Position pos;

    //Make blank position
    for (int i = 2; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            pos.position[j][i] = {PieceEnum::NOTHING, false};
        }
    }

    //Add pawns
    for (int i = 0; i < 8; i++)
    {
        pos.position[i][1] = {PieceEnum::PAWN, true};
        pos.position[i][6] = {PieceEnum::PAWN, false};
    }

    //Add ohter pieces
    //Rooks
    pos.position[0][0] = {PieceEnum::ROOK, true};
    pos.position[7][0] = {PieceEnum::ROOK, true};
    pos.position[0][7] = {PieceEnum::ROOK, false};
    pos.position[7][7] = {PieceEnum::ROOK, false};
    //Knights
    pos.position[1][0] = {PieceEnum::KNIGHT, true};
    pos.position[6][0] = {PieceEnum::KNIGHT, true};
    pos.position[1][7] = {PieceEnum::KNIGHT, false};
    pos.position[6][7] = {PieceEnum::KNIGHT, false};
    //Bishops
    pos.position[2][0] = {PieceEnum::BISHOP, true};
    pos.position[5][0] = {PieceEnum::BISHOP, true};
    pos.position[2][7] = {PieceEnum::BISHOP, false};
    pos.position[5][7] = {PieceEnum::BISHOP, false};
    //Queens
    pos.position[3][0] = {PieceEnum::QUEEN, true};
    pos.position[3][7] = {PieceEnum::QUEEN, false};
    //Kings
    pos.position[4][0] = {PieceEnum::KING, true};
    pos.position[4][7] = {PieceEnum::KING, false};
    pos.xWhiteKing = 4;
    pos.yWhiteKing = 0;
    pos.xBlackKing = 4;
    pos.yBlackKing = 7;
    //Board's complete

    return pos;
}

Game::Game()
{
    Position pos = setupPosition();

    pos.firstMove     = true;
    pos.isWhiteOnMove = true;
    
    positions.push_back(pos);
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

    std::vector<Move> pieceMoves = movement.findLegalMoves(moveVar.xStart, moveVar.yStart, pos);
    for (Move m : pieceMoves)
    {
        if (m == moveVar)
        {
            return makeMoveToGame(m);
        }
    }
    return true;
}

bool Game::makeMoveToGame(Move pMove)
{
    positions.push_back(movement.move(pMove, positions.back()));

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
    s += pos.inCheck ? "Check!\n" : "";
    s += pos.isWhiteOnMove ? "White to move...\n" : "Black to move...\n";
    return s;
}

Game::~Game()
{}