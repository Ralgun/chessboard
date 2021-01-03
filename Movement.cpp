#include "Movement.hpp"
#include "PieceEnum.hpp"
#include <math.h>
#include <algorithm>

Movement::Movement()
{

    diagonalMovementPieces.push_back(PieceEnum::BISHOP);
    diagonalMovementPieces.push_back(PieceEnum::QUEEN);
    orthogonalMovementPieces.push_back(PieceEnum::ROOK);
    orthogonalMovementPieces.push_back(PieceEnum::QUEEN);
    knightMovementPieces.push_back(PieceEnum::KNIGHT);
    pawnMovementPieces.push_back(PieceEnum::PAWN);

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

std::vector<Move> Movement::findLegalMovesForAllPieces(Position pos)
{
    std::vector<Move> moves;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            //Checks if friednly piece is on the square
            if (isSquareOccupiedByOpponent(i, j, !pos.isWhiteOnMove, pos))
            {
                std::vector<Move> m = findLegalMovesForPiece(i, j, pos);
                moves.insert(moves.end(), m.begin(), m.end());
            }
        }
    }

    return moves;
}

std::vector<Move> Movement::findLegalMovesForPiece(char x, char y, Position pos)
{
    std::vector<Move> moves;

    PieceReference p = pos.position[x][y];
    switch (p.piece)
    {
        case PieceEnum::PAWN:
            moves = findMovesForPawn(x, y, p.isWhite, pos);
        break;
        case PieceEnum::ROOK:
            moves = checkSlidingMovement(x, y, rookDirections, p.isWhite, pos, [](Move* m) {
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
            moves = checkKnightMovement(x, y, p.isWhite, pos, nullptr);
        break;
        case PieceEnum::BISHOP:
            moves = checkSlidingMovement(x, y, bishopDirections, p.isWhite, pos, nullptr);
        break;
        case PieceEnum::QUEEN:
            moves = checkSlidingMovement(x, y, queenDirections, p.isWhite, pos, nullptr);
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

    //Filter out illegal moves and calculate if move gives check (Merging these two functions might be a bad idea. Hopefuly, nothing bad comes from this)
    moves.erase(std::remove_if(moves.begin(), moves.end(), [&](Move& m) -> bool {
        
        char xKing, yKing, xEnemy, yEnemy;
        Position theoreticalPos = move(m, pos);
        if (pos.isWhiteOnMove)
        {
            xKing = theoreticalPos.xWhiteKing;
            yKing = theoreticalPos.yWhiteKing;
            xEnemy = theoreticalPos.xBlackKing;
            yEnemy = theoreticalPos.yBlackKing;
        }
        else
        {
            xKing = theoreticalPos.xBlackKing;
            yKing = theoreticalPos.yBlackKing;
            xEnemy = theoreticalPos.xWhiteKing;
            yEnemy = theoreticalPos.yWhiteKing;
        }
        if (isInCheck(xKing, yKing, pos.isWhiteOnMove, theoreticalPos))
        {
            return true;
        }
        //Check if move gives check
        m.givesCheck = isInCheck(xEnemy, yEnemy, !pos.isWhiteOnMove, theoreticalPos);
        return false;
    }), moves.end());

    return moves;
}

bool Movement::isInCheck(char x, char y, bool checkWhite, Position pos)
{
    //TODO look for double checks
    //Check for orthogonall movement (rooks, queens)
    for (Move m : checkSlidingMovement(x, y, rookDirections, checkWhite, pos, nullptr))
    {
        if (isSquareOccupiedBySpecificEnemyPiece(m.xEnd, m.yEnd, checkWhite, orthogonalMovementPieces, pos))
        {
            return true;
        }
    }
    //Check diagonal movement (queens, bishops)
    for (Move m : checkSlidingMovement(x, y, bishopDirections, checkWhite, pos, nullptr))
    {
        if (isSquareOccupiedBySpecificEnemyPiece(m.xEnd, m.yEnd, checkWhite, diagonalMovementPieces, pos))
        {
            return true;
        }
    }
    //Check knight movement
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
        if ((x+1 < 8 && y+1 < 8 && isSquareOccupiedBySpecificEnemyPiece(x+1, y+1, checkWhite, pawnMovementPieces, pos)) ||
            (x-1 >= 0 && y+1 < 8 && isSquareOccupiedBySpecificEnemyPiece(x-1, y+1, checkWhite, pawnMovementPieces, pos)))
        {
            return true;
        }
    }
    else 
    {
        if ((x+1 < 8 && y-1 >= 0 && isSquareOccupiedBySpecificEnemyPiece(x+1, y-1, checkWhite, pawnMovementPieces, pos)) ||
            (x-1 >= 0 && y-1 >= 0 && isSquareOccupiedBySpecificEnemyPiece(x-1, y-1, checkWhite, pawnMovementPieces, pos)))
        {
            return true;
        }
    }

    return false;
}

//TODO make readable
std::vector<Move> Movement::generateCastlingMoves(bool isWhite, Position pos)
{
    std::vector<Move> moves;

    char castlingValues[] = {2, -2};
    char rookPositions[]  = {1, -1};

    if (isWhite)
    {
        if (pos.whiteShortCastle && !isSquareOccupied(5, 0, pos) && !isSquareOccupied(6, 0, pos))
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
        if (pos.whiteLongCastle && !isSquareOccupied(3, 0, pos) && !isSquareOccupied(2, 0, pos) && !isSquareOccupied(1, 0, pos))
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
        if (pos.blackShortCastle && !isSquareOccupied(5, 7, pos) && !isSquareOccupied(6, 7, pos))
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
        if (pos.blackLongCastle && !isSquareOccupied(3, 7, pos) && !isSquareOccupied(2, 7, pos) && !isSquareOccupied(1, 7, pos))
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
                (isSquareEmptyOrEnemy(x+xD, y+yD, isWhite, pos)))
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
            if ((xNew < 8 && xNew >= 0) && (yNew < 8 && yNew >= 0) && 
                (isSquareEmptyOrEnemy(xNew, yNew, isWhite, pos)))
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
            if ((xNew < 8 && xNew >= 0) && (yNew < 8 && yNew >= 0) && 
                (isSquareEmptyOrEnemy(xNew, yNew, isWhite, pos)))
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
//^ That was a lie and it cost me several hours of life
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
    
    if (!isSquareOccupied(x, y+mult, pos))
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
            if (y == initialY && !isSquareOccupied(x, y+2*(mult), pos))
            {
                moves.push_back({x, y, x, y+(2*mult)});
            }
        }
    }
    //Captures
    if (x-1 >= 0 && isSquareOccupiedByOpponent(x-1, y+mult, isWhite, pos))
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
    if (x+1 < 8 && isSquareOccupiedByOpponent(x+1, y+mult, isWhite, pos))
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

Position Movement::move(Move pMove, Position pos)
{

    //Fetching current position
    PieceReference p = pos.position[pMove.xStart][pMove.yStart];
    //Promotion
    if (pMove.promotion != PieceEnum::NOTHING)
    {
        p.piece = pMove.promotion;
    }

    //Fifty move rule
    if (pos.position[pMove.xEnd][pMove.yEnd].piece != PieceEnum::NOTHING || p.piece == PieceEnum::PAWN)
    {
        //Refresh the counter
        pos.fiftyMoveRuleDeadline = pos.currentMoveNum + 100;
    }

    pos.position[pMove.xStart][pMove.yStart].piece = PieceEnum::NOTHING;
    pos.position[pMove.xEnd][pMove.yEnd] = p;

    if (pos.isWhiteOnMove)
    {
        pos.whiteShortCastle = !pMove.removeShortCastle & pos.whiteShortCastle;
        pos.whiteLongCastle = !pMove.removeLongCastle & pos.whiteLongCastle;
    }
    else
    {
        pos.blackShortCastle = !pMove.removeShortCastle & pos.blackShortCastle;
        pos.blackLongCastle = !pMove.removeLongCastle & pos.blackLongCastle;
    }

    //Special case: en passant
    if (pMove.enpassant)
    {
        char mult = pos.isWhiteOnMove ? -1 : 1;
        pos.position[pMove.xEnd][pMove.yEnd+mult].piece = PieceEnum::NOTHING;
    }
    //Special case: castling
    if (pMove.castle)
    {
        PieceReference rook = pos.position[pMove.oldRookX][pMove.oldRookY];
        pos.position[pMove.oldRookX][pMove.oldRookY].piece = PieceEnum::NOTHING;
        pos.position[pMove.newRookX][pMove.newRookY] = rook;
    }
    //Special case: king move
    if (p.piece == PieceEnum::KING)
    {
        if (pos.isWhiteOnMove)
        {
            pos.xWhiteKing = pMove.xEnd;
            pos.yWhiteKing = pMove.yEnd;
        }
        else
        {
            pos.xBlackKing = pMove.xEnd;
            pos.yBlackKing = pMove.yEnd;
        }
    }

    pos.inCheck = pMove.givesCheck;
    pos.isWhiteOnMove = !pos.isWhiteOnMove;
    pMove.moved = p.piece;
    pos.lastMove = pMove;
    pos.firstMove = false;
    pos.currentMoveNum++;

    return pos;
}

bool Movement::isSquareEmptyOrEnemy(char x, char y, bool isWhite, Position pos)
{
    return !isSquareOccupied(x, y, pos) || pos.position[x][y].isWhite != isWhite;
}

bool Movement::isSquareOccupied(char x, char y, Position pos)
{
    return pos.position[x][y].piece != PieceEnum::NOTHING;
}

bool Movement::isSquareOccupiedByOpponent(char x, char y, bool isWhite, Position pos)
{
    return pos.position[x][y].piece != PieceEnum::NOTHING && pos.position[x][y].isWhite != isWhite;
}

bool Movement::isSquareOccupiedBySpecificEnemyPiece(char x, char y, bool isWhite, std::vector<PieceEnum> pieces, Position pos)
{
    if (isSquareOccupiedByOpponent(x, y, isWhite, pos))
    {
        for (PieceEnum p : pieces)
        {
            if (pos.position[x][y].piece == p)
            {
                return true;
            }
        }
    }
    return false;
}