#pragma once

#include "Move.hpp"
#include "Position.hpp"
#include <vector>

class Movement
{
private:
    bool isSquareEmptyOrEnemy(char x, char y, bool isWhite, Position pos);
    bool isSquareOccupied(char x, char y, Position pos);
    bool isSquareOccupiedByOpponent(char x, char y, bool isWhite, Position pos);
    bool isSquareOccupiedBySpecificEnemyPiece(char x, char y, bool isWhite, std::vector<PieceEnum> pieces, Position pos);
    //Helper vectors for pieces
    std::vector<PieceEnum> diagonalMovementPieces;
    std::vector<PieceEnum> orthogonalMovementPieces;
    //I should think this trough
    std::vector<PieceEnum> knightMovementPieces;
    std::vector<PieceEnum> pawnMovementPieces;
    //Helper vectors for sliding movement
    std::vector<std::pair<char, char>> bishopDirections;
    std::vector<std::pair<char, char>> rookDirections;
    std::vector<std::pair<char, char>> queenDirections;
public:
    bool isMoveLegal(Move m, Position pos);
    bool isInCheck(char x, char y, bool isWhiteOnMove, Position pos);
    std::vector<Move> findMovesForPawn(char x, char y, bool isWhite, Position pos);
    std::vector<Move> generateCastlingMoves(bool isWhite, Position pos);
    std::vector<Move> checkKingMovement(char x, char y, bool isWhite, Position pos, void (*f)(Move* pMove));
    std::vector<Move> findLegalMovesForAllPieces(Position pos);
    std::vector<Move> findLegalMovesForPiece(char x, char y, Position pos);
    std::vector<Move> checkKnightMovement(char x, char y, bool isWhite, Position pos, void (*f)(Move* pMove));
    std::vector<Move> checkSlidingMovement(char x, char y, std::vector<std::pair<char, char>> directions, bool isWhite, Position pos, void (*f)(Move* pMove));
    Position move(Move move, Position pos);
    Movement();
};