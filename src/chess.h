#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <string>
#include <vector>
#include <array>

enum class Piece : uint8_t
{
    e, // empty square
    p, // white pawn
    r, // white rook
    n, // white knight
    b, // white bishop
    q, // white queen
    k, // white king
    P, // black pawn
    R, // black rook
    N, // black knight
    B, // black bishop
    Q, // black queen
    K, // black king
};

enum class Square : uint8_t
{
    a1,
    b1,
    c1,
    d1,
    e1,
    f1,
    g1,
    h1,
    a2,
    b2,
    c2,
    d2,
    e2,
    f2,
    g2,
    h2,
    a3,
    b3,
    c3,
    d3,
    e3,
    f3,
    g3,
    h3,
    a4,
    b4,
    c4,
    d4,
    e4,
    f4,
    g4,
    h4,
    a5,
    b5,
    c5,
    d5,
    e5,
    f5,
    g5,
    h5,
    a6,
    b6,
    c6,
    d6,
    e6,
    f6,
    g6,
    h6,
    a7,
    b7,
    c7,
    d7,
    e7,
    f7,
    g7,
    h7,
    a8,
    b8,
    c8,
    d8,
    e8,
    f8,
    g8,
    h8
};

class ChessGame
{
public:
    struct Move
    {
        Square from;
        Square to;
        Piece promotion;
        bool isPromotion;
        bool isCapture;
        bool isEnPassant;
        bool isCastling;
    };

    ChessGame();
    void printBoard() const;
    bool makeMove(const std::string &move);
    bool isGameOver() const;
    void parseFEN(const std::string &fen);
    std::string generateFEN() const;

    std::vector<Move> generateMoves() const;

    void addMovesFromBitboard(std::vector<Move> &moves, uint64_t moveBitboard,
                              int pieceType, int offsetForSource) const;

private:
    // TODO: Define your board representation here (e.g., array or vector)
    // For simplicity, we can use a 2D vector of small ints to represent the board

    // Board will follow this order:
    /* {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
    };
    */
    Piece boardArray[8][8] = {
        {Piece::r, Piece::n, Piece::b, Piece::q, Piece::k, Piece::b, Piece::n, Piece::r},
        {Piece::p, Piece::p, Piece::p, Piece::p, Piece::p, Piece::p, Piece::p, Piece::p},
        {Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e},
        {Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e},
        {Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e},
        {Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e, Piece::e},
        {Piece::P, Piece::P, Piece::P, Piece::P, Piece::P, Piece::P, Piece::P, Piece::P},
        {Piece::R, Piece::N, Piece::B, Piece::Q, Piece::K, Piece::B, Piece::N, Piece::R}};

    // Bitboards
    uint64_t pieceBitboards[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // prnbqkPRNBQK
    uint64_t occupiedBitboard = 0;
    uint64_t emptyBitboard = 0;

    bool whiteTurn = 1;

    bool castlingRights[4] = {true, true, true, true}; // KQkq

    std::string enPassantTargetSquare;

    int halfmoveClock = 0;  // Halfmove clock for the fifty-move rule
    int fullmoveNumber = 1; // Fullmove number

    // Set up constants for the bitboard representation
    uint64_t rankConst[8] = {
        0x00000000000000FF,
        0x000000000000FF00,
        0x0000000000FF0000,
        0x00000000FF000000,
        0x000000FF00000000,
        0x0000FF0000000000,
        0x00F0000000000000,
        0xFF00000000000000};

    uint64_t fileConst[8] = {
        0x0101010101010101,
        0x0202020202020202,
        0x0404040404040404,
        0x0808080808080808,
        0x1010101010101010,
        0x2020202020202020,
        0x4040404040404040,
        0x8080808080808080};

    // Precomputed attack bitboards

    uint64_t knightPseudoAttacks[64];
    void initializeKnightAttacks();

    bool isValidMove(const std::string &move) const;
    void applyMove(const std::string &move);

    void resetBoard();

    void printBitboards() const;

    int pop_lsb(uint64_t &bitboard) const;
};

#endif // CHESS_H