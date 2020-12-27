#include "Movement.hpp"
#include "PieceEnum.hpp"
#include <math.h>

Movement::Movement(Game* pGame)
{
    game = pGame;

    rookDirections.push_back(std::pair<char, char>(1,0));
    rookDirections.push_back(std::pair<char, char>(0,1));
    rookDirections.push_back(std::pair<char, char>(-1,0));
    rookDirections.push_back(std::pair<char, char>(0,-1));

    bishopDirections.push_back(std::pair<char, char>(1, 1));
    bishopDirections.push_back(std::pair<char, char>(1, -1));
    bishopDirections.push_back(std::pair<char, char>(-1, 1));
    bishopDirections.push_back(std::pair<char, char>(-1, -1));

    //https://stackoverflow.com/questions/3177241/what-is-the-best-way-to-concatenate-two-vectors
    queenDirections.reserve(rookDirections.size() + bishopDirections.size());
    queenDirections.insert(queenDirections.end(), rookDirections.begin(), rookDirections.end());
    queenDirections.insert(queenDirections.end(), bishopDirections.begin(), bishopDirections.end());
}

std::vector<Move> Movement::findLegalMoves(char x, char y, Position pos)
{
    std::vector<Move> moves;

    PieceReference p = pos.position[x][y];
    switch (p.piece)
    {
        case PieceEnum::PAWN:
            return findMovesForPawn(x, y, p.isWhite, pos);
        break;
        case PieceEnum::ROOK:
            return checkSlidingMovement(x, y, rookDirections, p.isWhite, pos, [](Move* m) {
                if (m->xStart == 7)
                {
                    m->removeShortCastle = true;
                }
                else if (m->xStart == 0)
                {
                    m->removeLongCastle = true;
                }
            });
        break;
        case PieceEnum::KNIGHT:
            return checkKnightMovement(x, y, p.isWhite, pos, nullptr);
        break;
        case PieceEnum::BISHOP:
            return checkSlidingMovement(x, y, bishopDirections, p.isWhite, pos, nullptr);
        break;
        case PieceEnum::QUEEN:
            return checkSlidingMovement(x, y, queenDirections, p.isWhite, pos, nullptr);
        break;
        case PieceEnum::KING:
            moves = checkKingMovement(x, y, p.isWhite, pos, [](Move* m) {
                m->removeLongCastle = true;
                m->removeShortCastle = true;
            });
            std::vector<Move> b = generateCastlingMoves(p.isWhite, pos);
            moves.insert(moves.end(), b.begin(), b.end());
        break;
    }

    return moves;
}

bool Movement::isInCheck(char x, char y, bool checkWhite, Position pos)
{
    //TODO look for double checks
    //Check for orthogonall movement (rooks, queens)
    Move m = checkSlidingMovement(x, y, rookDirections, checkWhite, pos, nullptr).back();
    if (pos.position[m.xEnd][m.yEnd].piece == PieceEnum::ROOK || pos.position[m.xEnd][m.yEnd].piece == PieceEnum::QUEEN)
    {
        return true;
    }
    //Check diagonal movement (queens, bishops)
    Move m = checkSlidingMovement(x, y, bishopDirections, checkWhite, pos, nullptr).back();
    if (pos.position[m.xEnd][m.yEnd].piece == PieceEnum::BISHOP || pos.position[m.xEnd][m.yEnd].piece == PieceEnum::QUEEN)
    {
        return true;
    }
    //Check knight movement
    //Since checkKnightMovement() doesn't stop once it finds oponent's piece, we need to go trough each move
    for (Move m : checkKnightMovement(x, y, checkWhite, pos, nullptr))
    {
        if (pos.position[m.xEnd][m.yEnd].piece == PieceEnum::KNIGHT)
        {
            return true;
        }
    }
    //Check pawn movement
    //It looks bad, but partially calculating pawn's movement avoids using entire findMovesForPawn(). findMovesForPawn() should be probably split
    if (checkWhite)
    {
        if ((pos.position[x+1][y+1].piece == PieceEnum::PAWN && pos.position[x+1][y+1].isWhite == !checkWhite) ||
            (pos.position[x-1][y+1].piece == PieceEnum::PAWN && pos.position[x-1][y+1].isWhite == !checkWhite))
        {
            return true;
        }
    }
    else 
    {
        if ((pos.position[x+1][y-1].piece == PieceEnum::PAWN && pos.position[x+1][y-1].isWhite == !checkWhite) ||
            (pos.position[x-1][y-1].piece == PieceEnum::PAWN && pos.position[x-1][y-1].isWhite == !checkWhite))
        {
            return true;
        }
    }

    return false;
}

std::vector<Move> Movement::generateCastlingMoves(bool isWhite, Position pos)
{
    std::vector<Move> moves;

    char castlingValues[] = {2, -2};
    char rookPositions[]  = {1, -1};

    if (isWhite)
    {
        if (pos.whiteShortCastle && pos.position[5][0].piece == PieceEnum::NOTHING && pos.position[6][0].piece == PieceEnum::NOTHING)
        {
            Move m = {4, 0, 6, 0};
            m.castle = true;
            m.removeLongCastle;
            m.removeShortCastle;
            m.oldRookX = 7;
            m.oldRookY = 0;
            m.newRookX = 5;
            m.newRookY = 0;
            m.moved = PieceEnum::KING;
            moves.push_back(m);
        }
        if (pos.whiteLongCastle && pos.position[3][0].piece == PieceEnum::NOTHING && pos.position[2][0].piece == PieceEnum::NOTHING && pos.position[1][0].piece == PieceEnum::NOTHING)
        {
            Move m = {4, 0, 2, 0};
            m.castle = true;
            m.removeLongCastle;
            m.removeShortCastle;
            m.oldRookX = 0;
            m.oldRookY = 0;
            m.newRookX = 3;
            m.newRookY = 0;
            m.moved = PieceEnum::KING;
            moves.push_back(m);
        }
    }
    else
    {
        if (pos.blackShortCastle && pos.position[5][7].piece == PieceEnum::NOTHING && pos.position[6][7].piece == PieceEnum::NOTHING)
        {
            Move m = {4, 7, 6, 7};
            m.castle = true;
            m.removeLongCastle;
            m.removeShortCastle;
            m.oldRookX = 7;
            m.oldRookY = 7;
            m.newRookX = 5;
            m.newRookY = 7;
            m.moved = PieceEnum::KING;
            moves.push_back(m);
        }
        if (pos.blackLongCastle && pos.position[3][7].piece == PieceEnum::NOTHING && pos.position[2][7].piece == PieceEnum::NOTHING && pos.position[1][7].piece == PieceEnum::NOTHING)
        {
            Move m = {4, 7, 2, 7};
            m.castle = true;
            m.removeLongCastle;
            m.removeShortCastle;
            m.oldRookX = 0;
            m.oldRookY = 7;
            m.newRookX = 3;
            m.newRookY = 7;
            m.moved = PieceEnum::KING;
            moves.push_back(m);
        }
    }

    return moves;
}

std::vector<Move> Movement::checkKingMovement(char x, char y, bool isWhite, Position pos, void (*f)(Move* pMove))
{
    std::vector<Move> moves;

    char a[] = {1, 0, -1};
    for (char xD : a)
    {
        for (char yD : a)
        {
            if ((x+xD >= 0 && x+xD < 8 && y+yD >= 0 && y+yD < 8) && 
                ((pos.position[x+xD][y+yD].piece == PieceEnum::NOTHING) || (pos.position[x+xD][y+yD].isWhite != isWhite)))
            {
                Move m = {x, y, x+xD, y+yD};
                if (f != nullptr)
                {
                    f(&m);
                }
                moves.push_back(m);
            }
        }
    }

    return moves;
}

std::vector<Move> Movement::checkKnightMovement(char x, char y, bool isWhite, Position pos, void (*f)(Move* pMove))
{
    std::vector<Move> moves;

    char longDirections[]  = {2, -2};    
    char shortDirections[] = {1, -1};

    for (char longerDir : longDirections)
    {
        for (char shorterDir : shortDirections)
        {
            char xNew = x + longerDir;
            char yNew = y + shorterDir;
            PieceReference p = pos.position[xNew][yNew];
            if ((xNew < 8 && xNew >= 0) && (yNew < 8 && yNew >= 0) && 
                (p.piece == PieceEnum::NOTHING || (p.isWhite != isWhite)))
            {
                Move m = {x, y, xNew, yNew};
                if (f != nullptr)
                {
                    f(&m);
                }
                moves.push_back(m);
            }
            xNew = x + shorterDir;
            yNew = y + longerDir;
            p = pos.position[xNew][yNew];
            if ((xNew < 8 && xNew >= 0) && (yNew < 8 && yNew >= 0) && 
                (p.piece == PieceEnum::NOTHING || (p.isWhite != isWhite)))
            {
                Move m = {x, y, xNew, yNew};
                if (f != nullptr)
                {
                    f(&m);
                }
                moves.push_back(m);
            }
        }
    }

    return moves;
}

std::vector<Move> Movement::checkSlidingMovement(char x, char y, std::vector<std::pair<char, char>> directions, bool isWhite, Position pos, void (*f)(Move* pMove))
{
    std::vector<Move> moves;

    //Every array in directions should have an array with two elements corresponding to x and y value difference
    for (std::pair<char, char> direction : directions)
    {
        //We start from one because we don't care about piece's starting position
        for (char i = 1; i < 8; i++)
        {
            char xNew = x + direction.first * i;
            char yNew = y + direction.second * i;

            if ((xNew < 0 || xNew > 7) || (yNew < 0 || yNew > 7))
            {
                break;
            }
            //Friendly piece blocks
            PieceReference p = pos.position[xNew][yNew];
            if (p.piece != PieceEnum::NOTHING && p.isWhite == isWhite)
            {
                break;
            }

            //Create move and call callback
            Move m = {x, y, xNew, yNew};
            if (f != nullptr)
            {
                f(&m);
            }
            moves.push_back(m);

            //Enemy piece blocks but we can still take it
            if (p.piece != PieceEnum::NOTHING)
            {
                break;
            }
        }
    }

    return moves;
}

//I didn't check this code I just know it works
std::vector<Move> Movement::findMovesForPawn(char x, char y, bool isWhite, Position pos)
{
    std::vector<Move> moves;
    char mult     = 1;
    char initialY = 1;
    char enemyY   = 6;
    if (!isWhite)
    {
        mult     = -1;
        initialY =  6;
        enemyY   =  1;
    }
    
    if (pos.position[x][y+mult].piece == PieceEnum::NOTHING)
    {
        //Promotion
        if (y == enemyY)
        {
            Move m = {x, y, x, y+mult};
            m.promotion = PieceEnum::QUEEN;
            moves.push_back(m);
            m.promotion = PieceEnum::ROOK;
            moves.push_back(m);
            m.promotion = PieceEnum::KNIGHT;
            moves.push_back(m);
            m.promotion = PieceEnum::BISHOP;
            moves.push_back(m);
        }
        else 
        {
            moves.push_back({x, y, x, y+mult});
            if (y == initialY && pos.position[x][y+2*(mult)].piece == PieceEnum::NOTHING)
            {
                moves.push_back({x, y, x, y+(2*mult)});
            }
        }
    }
    //Captures
    if (pos.position[x-1][y+mult].piece != PieceEnum::NOTHING && pos.position[x-1][y+mult].isWhite != isWhite)
    {
        if (y == enemyY)
        {
            Move m = {x, y, x-1, y+mult};
            m.promotion = PieceEnum::QUEEN;
            moves.push_back(m);
            m.promotion = PieceEnum::ROOK;
            moves.push_back(m);
            m.promotion = PieceEnum::KNIGHT;
            moves.push_back(m);
            m.promotion = PieceEnum::BISHOP;
            moves.push_back(m);
        }
        else
        {
            moves.push_back({x, y, x-1, y+mult});
        }
    }
    if (pos.position[x+1][y+mult].piece != PieceEnum::NOTHING && pos.position[x+1][y+mult].isWhite != isWhite)
    {
        if (y == enemyY)
        {
            Move m = {x, y, x+1, y+mult};
            m.promotion = PieceEnum::QUEEN;
            moves.push_back(m);
            m.promotion = PieceEnum::ROOK;
            moves.push_back(m);
            m.promotion = PieceEnum::KNIGHT;
            moves.push_back(m);
            m.promotion = PieceEnum::BISHOP;
            moves.push_back(m);
        }
        else
        {
            moves.push_back({x, y, x+1, y+mult});
        }
    }

    //En passant
    if (pos.lastMove.moved == PieceEnum::PAWN && abs(pos.lastMove.xEnd - x) == 1 &&
        pos.lastMove.yStart == enemyY && y == pos.lastMove.yEnd)
    {
        Move m = {x, y, pos.lastMove.xEnd, y+mult};
        m.enpassant = true;
        moves.push_back(m);
    }

    return moves;
}