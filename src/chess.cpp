#include "./chess.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>

ChessGame::ChessGame() : whiteTurn(true)
{
    // Initialize the chessboard with pieces in their starting positions
    initializeKnightAttacks();
    initializeKingAttacks();
    initializeRayAttacks();
}

void ChessGame::printBoard(bool withBitboards) const
{
    // Print the chessboard to the console
    std::cout << "Chessboard:" << std::endl;
    for (int row = 7; row >= 0; --row)
    {
        for (int col = 0; col < 8; ++col)
        {
            switch (boardArray[row][col])
            {
            case Piece::e:
                std::cout << ". ";
                break;
            case Piece::p:
                std::cout << "p ";
                break;
            case Piece::r:
                std::cout << "r ";
                break;
            case Piece::n:
                std::cout << "n ";
                break;
            case Piece::b:
                std::cout << "b ";
                break;
            case Piece::q:
                std::cout << "q ";
                break;
            case Piece::k:
                std::cout << "k ";
                break;
            case Piece::P:
                std::cout << "P ";
                break;
            case Piece::R:
                std::cout << "R ";
                break;
            case Piece::N:
                std::cout << "N ";
                break;
            case Piece::B:
                std::cout << "B ";
                break;
            case Piece::Q:
                std::cout << "Q ";
                break;
            case Piece::K:
                std::cout << "K ";
                break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "White's turn: " << (whiteTurn ? "Yes" : "No") << std::endl;
    std::cout << "Game over: " << (isGameOver() ? "Yes" : "No") << std::endl;
    std::cout << "Castling rights: ";
    std::cout << (castlingRights[0] ? "K" : "-");
    std::cout << (castlingRights[1] ? "Q" : "-");
    std::cout << (castlingRights[2] ? "k" : "-");
    std::cout << (castlingRights[3] ? "q" : "-");
    std::cout << std::endl;
    std::cout << "En passant target square: " << (enPassantTargetSquare.empty() ? "-" : enPassantTargetSquare) << std::endl;
    std::cout << "Halfmove clock: " << halfmoveClock << std::endl;
    std::cout << "Fullmove number: " << fullmoveNumber << std::endl;
    std::cout << "------------------------" << std::endl;
    if (withBitboards)
    {
        printBitboards();
    }
}

bool ChessGame::makeMove(const std::string &move)
{
    if (!isValidMove(move))
    {
        return false;
    }
    applyMove(move);
    whiteTurn = !whiteTurn;
    preworkPosition(); // Update the board state after the move
    return true;
}

bool ChessGame::isValidMove(const std::string &move) const
{
    // TODO: Validate syntax and legal move
    return true;
}

void ChessGame::applyMove(const std::string &move)
{
    // TODO: Update board based on move
}

bool ChessGame::isGameOver() const
{
    // TODO: Check for checkmate, stalemate, etc.
    return false;
}

// TODO: Add error handling for invalid FEN strings
void ChessGame::parseFEN(const std::string &fen)
{
    resetBoard(); // Reset the board before parsing a new FEN string

    std::istringstream ss(fen);
    std::string board, activeColor, castling, enPassant, halfmove, fullmove;

    // Parse the 6 fields
    ss >> board >> activeColor >> castling >> enPassant >> halfmove >> fullmove;

    // Process board placement
    int row = 0;
    int col = 0;

    int i = 0;
    for (char c : board)
    {
        if (c == '/')
        {
            // Move to the next rank
            row++;
            col = 0;
        }
        else if (isdigit(c))
        {
            // Skip empty squares
            col += c - '0';
        }
        else
        {
            // Place a piece
            Piece piece;
            switch (c)
            {
            case 'p':
                piece = Piece::p;
                pieceBitboards[0] |= (1ULL << (row * 8 + col));
                break;
            case 'r':
                piece = Piece::r;
                pieceBitboards[1] |= (1ULL << (row * 8 + col));
                break;
            case 'n':
                piece = Piece::n;
                pieceBitboards[2] |= (1ULL << (row * 8 + col));
                break;
            case 'b':
                piece = Piece::b;
                pieceBitboards[3] |= (1ULL << (row * 8 + col));
                break;
            case 'q':
                piece = Piece::q;
                pieceBitboards[4] |= (1ULL << (row * 8 + col));
                break;
            case 'k':
                piece = Piece::k;
                pieceBitboards[5] |= (1ULL << (row * 8 + col));
                break;
            case 'P':
                piece = Piece::P;
                pieceBitboards[6] |= (1ULL << (row * 8 + col));
                break;
            case 'R':
                piece = Piece::R;
                pieceBitboards[7] |= (1ULL << (row * 8 + col));
                break;
            case 'N':
                piece = Piece::N;
                pieceBitboards[8] |= (1ULL << (row * 8 + col));
                break;
            case 'B':
                piece = Piece::B;
                pieceBitboards[9] |= (1ULL << (row * 8 + col));
                break;
            case 'Q':
                piece = Piece::Q;
                pieceBitboards[10] |= (1ULL << (row * 8 + col));
                break;
            case 'K':
                piece = Piece::K;
                pieceBitboards[11] |= (1ULL << (row * 8 + col));
                break;
            default:
                piece = Piece::e; // Should not happen
            }
            boardArray[row][col] = piece;
            col++;
        }
    }

    // Set active color
    whiteTurn = (activeColor == "w");

    // NOTE: Currently not handling castling rights, en passant, or move counters
    // Castling rights can be set based on the castling string
    castlingRights[0] = (castling.find('K') != std::string::npos); // White kingside
    castlingRights[1] = (castling.find('Q') != std::string::npos); // White queenside
    castlingRights[2] = (castling.find('k') != std::string::npos); // Black kingside
    castlingRights[3] = (castling.find('q') != std::string::npos); // Black queenside

    // Set en passant target square
    if (enPassant == "-")
    {
        enPassantTargetSquare = "";
    }
    else
    {
        enPassantTargetSquare = enPassant;
    }

    halfmoveClock = std::stoi(halfmove);
    fullmoveNumber = std::stoi(fullmove);

    // Update occupied and empty bitboards
    for (int i = 0; i < 12; ++i)
    {
        if (pieceBitboards[i] != 0)
        {
            occupiedBitboard |= pieceBitboards[i];
        }
    }
    emptyBitboard = ~occupiedBitboard;
}

std::string ChessGame::generateFEN() const
{
    std::string fen;

    // Generate board representation
    for (int row = 0; row < 8; ++row)
    {
        int emptyCount = 0;
        for (int col = 0; col < 8; ++col)
        {
            if (boardArray[row][col] == Piece::e)
            {
                emptyCount++;
            }
            else
            {
                if (emptyCount > 0)
                {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                switch (boardArray[row][col])
                {
                case Piece::p:
                    fen += 'p';
                    break;
                case Piece::r:
                    fen += 'r';
                    break;
                case Piece::n:
                    fen += 'n';
                    break;
                case Piece::b:
                    fen += 'b';
                    break;
                case Piece::q:
                    fen += 'q';
                    break;
                case Piece::k:
                    fen += 'k';
                    break;
                case Piece::P:
                    fen += 'P';
                    break;
                case Piece::R:
                    fen += 'R';
                    break;
                case Piece::N:
                    fen += 'N';
                    break;
                case Piece::B:
                    fen += 'B';
                    break;
                case Piece::Q:
                    fen += 'Q';
                    break;
                case Piece::K:
                    fen += 'K';
                    break;
                default:
                    fen += 'e'; // Should not happen
                    break;
                }
            }
        }
        if (emptyCount > 0)
        {
            fen += std::to_string(emptyCount);
        }
        if (row < 7)
        {
            fen += '/';
        }
    }

    // Add active color
    fen += whiteTurn ? " w " : " b ";

    // Add castling rights
    if (castlingRights[0])
        fen += "K";
    if (castlingRights[1])
        fen += "Q";
    if (castlingRights[2])
        fen += "k";
    if (castlingRights[3])
        fen += "q";
    if (!castlingRights[0] && !castlingRights[1] && !castlingRights[2] && !castlingRights[3])
        fen += "-";

    // Add en passant target square
    if (enPassantTargetSquare.empty())
    {
        fen += " - ";
    }
    else
    {
        fen += " " + enPassantTargetSquare + " ";
    }
    // Add halfmove clock
    fen += std::to_string(halfmoveClock) + " ";
    // Add fullmove number
    fen += std::to_string(fullmoveNumber);
    return fen;
}

void ChessGame::resetBoard()
{
    // Reset the board to the initial position
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            boardArray[row][col] = Piece::e;
        }
    }
}

void ChessGame::printBitboards() const
{
    std::cout << "Bitboards:" << std::endl;
    for (int i = 0; i < 12; ++i)
    {
        std::cout << "Piece " << i << ": " << std::endl;
        printBitboard(pieceBitboards[i]);
        std::cout << "------------------------" << std::endl;
    }
    std::cout << "Occupied Bitboard: " << std::endl;
    printBitboard(occupiedBitboard);
    std::cout << "------------------------" << std::endl;
    std::cout << "Empty Bitboard: " << std::endl;
    printBitboard(emptyBitboard);
    std::cout << "------------------------" << std::endl;
}

std::vector<ChessGame::Move> ChessGame::generateMoves() const
{
    std::vector<Move> moves;

    uint64_t whitePieceBitboard = pieceBitboards[0] | pieceBitboards[1] | pieceBitboards[2] | pieceBitboards[3] | pieceBitboards[4] | pieceBitboards[5];

    uint64_t blackPieceBitboard = pieceBitboards[6] | pieceBitboards[7] | pieceBitboards[8] | pieceBitboards[9] | pieceBitboards[10] | pieceBitboards[11];

    if (whiteTurn)
    {
        // Generate moves for white pieces

        // Start with pawn moves

        // Pawn move by 1 square moves

        // TODO: Pawn promotion moves

        generateKingMoves(moves); // Generate king moves first to check for checks

        if (__builtin_popcountll(checkInfoStruct.checkers) >= 2) // King is in double check -> only king moves
        {
            return moves;
        }

        generateKnightMoves(moves);
        generateBishopMoves(moves);
        generateRookMoves(moves);
        generateQueenMoves(moves);

        // Castling moves
        if (!checkInfoStruct.isInCheck)
        {
            generateCastlingMoves(moves);
        }

        return moves;
    }
    else
    {
        // Generate moves for black pieces
    }

    return moves;
}

void ChessGame::generatePawnMoves(std::vector<Move> &moves) const
{
    // Generate pawn moves for the current turn
    uint64_t pawnBitboard = pieceBitboards[whiteTurn ? 0 : 6]; // Assuming 0 is the index for white pawns and 6 for black pawns
    int pawnSq;

    while (pawnBitboard)
    {
        pawnSq = pop_lsb(pawnBitboard); // Get the least significant bit (first pawn)

        // Forward move
        int forwardSq = pawnSq + (whiteTurn ? 8 : -8);
        if ((whiteTurn && forwardSq < 64 && boardArray[forwardSq / 8][forwardSq % 8] == Piece::e) ||
            (!whiteTurn && forwardSq >= 0 && boardArray[forwardSq / 8][forwardSq % 8] == Piece::e))
        {
            Move move;
            move.from = static_cast<Square>(pawnSq);
            move.to = static_cast<Square>(forwardSq);
            move.isCapture = false;
            moves.push_back(move);

            // Double forward move
            int doubleForwardSq = forwardSq + (whiteTurn ? 8 : -8);
            if ((whiteTurn && doubleForwardSq < 64 && boardArray[doubleForwardSq / 8][doubleForwardSq % 8] == Piece::e &&
                 (pawnSq / 8 == (whiteTurn ? 1 : 6))) ||
                (!whiteTurn && doubleForwardSq >= 0 && boardArray[doubleForwardSq / 8][doubleForwardSq % 8] == Piece::e &&
                 (pawnSq / 8 == (whiteTurn ? 6 : 1))))
            {
                Move doubleMove;
                doubleMove.from = static_cast<Square>(pawnSq);
                doubleMove.to = static_cast<Square>(doubleForwardSq);
                doubleMove.isCapture = false;
                moves.push_back(doubleMove);
            }
        }
    }
    // Capture moves
}

void ChessGame::generateKnightMoves(std::vector<Move> &moves) const
{
    // Generate knight moves for the current turn
    uint64_t knightBitboard = pieceBitboards[whiteTurn ? 2 : 8] & ~pinInfoStruct.pinned_pieces; // Assuming 2 is the index for white knights and 8 for black knights
    int knightSq;

    while (knightBitboard)
    {
        knightSq = pop_lsb(knightBitboard); // Get the least significant bit (first knight)

        // Get all possible attacks for this knight
        uint64_t attacks = knightPseudoAttacks[knightSq] & ~(whiteTurn ? whitePieces : blackPieces);

        while (attacks)
        {
            int destSq = pop_lsb(attacks); // Get the least significant bit (first attack)

            Move move;
            move.from = static_cast<Square>(knightSq);
            move.to = static_cast<Square>(destSq);
            move.isCapture = ((whiteTurn ? blackPieces : whitePieces) & (1ULL << destSq)) != 0; // Check if it's a capture
            moves.push_back(move);
        }
    }
}

void ChessGame::generateRookMoves(std::vector<Move> &moves) const
{
    // Generate rook moves for the current turn
    uint64_t rookBitboard = pieceBitboards[whiteTurn ? 1 : 7]; // Assuming 1 is the index for rooks
    int rookSq;

    while (rookBitboard)
    {
        rookSq = pop_lsb(rookBitboard); // Get the least significant bit (first rook)

        // Get all possible attacks for this rook
        uint64_t attacks = getRookAttacks(occupiedBitboard, rookSq) & ~(whiteTurn ? whitePieces : blackPieces);

        while (attacks)
        {
            int destSq = pop_lsb(attacks); // Get the least significant bit (first attack)

            Move move;
            move.from = static_cast<Square>(rookSq);
            move.to = static_cast<Square>(destSq);
            move.isCapture = ((whiteTurn ? blackPieces : whitePieces) & (1ULL << destSq)) != 0; // Check if it's a capture
            moves.push_back(move);
        }
    }
}

void ChessGame::generateBishopMoves(std::vector<Move> &moves) const
{
    // Generate bishop moves for the current turn
    uint64_t bishopBitboard = pieceBitboards[whiteTurn ? 3 : 9]; // Assuming 3 is the index for bishops
    int bishopSq;

    while (bishopBitboard)
    {
        bishopSq = pop_lsb(bishopBitboard); // Get the least significant bit (first bishop)

        // Get all possible attacks for this bishop
        uint64_t attacks = getBishopAttacks(occupiedBitboard, bishopSq) & ~(whiteTurn ? whitePieces : blackPieces);

        while (attacks)
        {
            int destSq = pop_lsb(attacks); // Get the least significant bit (first attack)

            Move move;
            move.from = static_cast<Square>(bishopSq);
            move.to = static_cast<Square>(destSq);
            move.isCapture = ((whiteTurn ? blackPieces : whitePieces) & (1ULL << destSq)) != 0; // Check if it's a capture
            moves.push_back(move);
        }
    }
}

void ChessGame::generateQueenMoves(std::vector<Move> &moves) const
{
    // Generate queen moves for the current turn
    uint64_t queenBitboard = pieceBitboards[whiteTurn ? 4 : 10]; // Assuming 4 is the index for queens
    int queenSq;

    while (queenBitboard)
    {
        queenSq = pop_lsb(queenBitboard); // Get the least significant bit (first queen)

        // Get all possible attacks for this queen
        uint64_t attacks = getQueenAttacks(occupiedBitboard, queenSq) & ~(whiteTurn ? whitePieces : blackPieces);

        while (attacks)
        {
            int destSq = pop_lsb(attacks); // Get the least significant bit (first attack)

            Move move;
            move.from = static_cast<Square>(queenSq);
            move.to = static_cast<Square>(destSq);
            move.isCapture = ((whiteTurn ? blackPieces : whitePieces) & (1ULL << destSq)) != 0; // Check if it's a capture
            moves.push_back(move);
        }
    }
}

void ChessGame::generateKingMoves(std::vector<Move> &moves) const // Only legal moves
{
    // Generate king moves for the current turn
    uint64_t kingBitboard = pieceBitboards[whiteTurn ? 5 : 11]; // Assuming 5 is the index for kings
    int kingSq;

    while (kingBitboard)
    {
        kingSq = pop_lsb(kingBitboard); // Get the least significant bit (first king)

        // Get all possible attacks for this king
        uint64_t attacks = kingPseudoAttacks[kingSq] & ~(whiteTurn ? whitePieces : blackPieces);

        while (attacks)
        {
            int destSq = pop_lsb(attacks); // Get the least significant bit (first attack)

            Move move;
            move.from = static_cast<Square>(kingSq);
            move.to = static_cast<Square>(destSq);
            move.isCapture = ((whiteTurn ? blackPieces : whitePieces) & (1ULL << destSq)) != 0; // Check if it's a capture
            moves.push_back(move);
        }
    }
}

void ChessGame::generateCastlingMoves(std::vector<Move> &moves) const
{
    // Generate castling moves for the current turn
    if (whiteTurn)
    {
        if (castlingRights[0]) // White kingside castling
        {
            Move move;
            move.from = Square::e1;
            move.to = Square::g1;
            move.isCastling = true;
            moves.push_back(move);
        }
        if (castlingRights[1]) // White queenside castling
        {
            Move move;
            move.from = Square::e1;
            move.to = Square::c1;
            move.isCastling = true;
            moves.push_back(move);
        }
    }
    else
    {
        if (castlingRights[2]) // Black kingside castling
        {
            Move move;
            move.from = Square::e8;
            move.to = Square::g8;
            move.isCastling = true;
            moves.push_back(move);
        }
        if (castlingRights[3]) // Black queenside castling
        {
            Move move;
            move.from = Square::e8;
            move.to = Square::c8;
            move.isCastling = true;
            moves.push_back(move);
        }
    }
}

// Method to extract moves from a bitboard of destinations
void ChessGame::addMovesFromBitboard(std::vector<Move> &moves, uint64_t moveBitboard,
                                     int pieceType, int offsetForSource) const
{
    // Loop through each set bit in the moveBitboard
    while (moveBitboard)
    {
        // Get position of least significant bit (first set bit)
        int destSq = __builtin_ctzll(moveBitboard); // count trailing zeros

        // Calculate source square (for pawns, go back based on offset)
        int sourceSq = destSq - offsetForSource;

        // Create and add the move
        Move move;
        move.from = static_cast<Square>(sourceSq);
        move.to = static_cast<Square>(destSq);
        moves.push_back(move);

        // Clear the processed bit
        moveBitboard &= moveBitboard - 1; // Clear least significant bit
    }
}

void ChessGame::initializeKnightAttacks()
{
    // Initialize knight pseudo-attack bitboards
    for (int i = 0; i < 64; ++i)
    {
        uint64_t attacks = 0;
        uint64_t bitboardWithPiece = (1ULL << i);

        if (bitboardWithPiece >> 17 & ~fileConst[7]) // Down-Down-Left
            attacks |= bitboardWithPiece >> 17;
        if (bitboardWithPiece >> 15 & ~fileConst[0]) // Down-Down-Right
            attacks |= bitboardWithPiece >> 15;
        if (bitboardWithPiece >> 10 & ~(fileConst[7] | fileConst[6])) // Down-Left-Left
            attacks |= bitboardWithPiece >> 10;
        if (bitboardWithPiece >> 6 & ~(fileConst[0] | fileConst[1])) // Down-Right-Right
            attacks |= bitboardWithPiece >> 6;
        if (bitboardWithPiece << 17 & ~fileConst[0]) // Up-Up-Right
            attacks |= bitboardWithPiece << 17;
        if (bitboardWithPiece << 15 & ~fileConst[7]) // Up-Up-Left
            attacks |= bitboardWithPiece << 15;
        if (bitboardWithPiece << 10 & ~(fileConst[0] | fileConst[1])) // Up-Right-Right
            attacks |= bitboardWithPiece << 10;
        if (bitboardWithPiece << 6 & ~(fileConst[7] | fileConst[6])) // Up-Left-Left
            attacks |= bitboardWithPiece << 6;

        knightPseudoAttacks[i] = attacks;
    }
    return;
}

void ChessGame::initializeKingAttacks()
{
    // Initialize king pseudo-attack bitboards
    for (int i = 0; i < 64; ++i)
    {
        uint64_t attacks = 0;
        uint64_t bitboardWithPiece = (1ULL << i);

        // King can move one square in any direction
        if (i % 8 != 7)                        // Not on the right edge
            attacks |= bitboardWithPiece << 1; // Right
        if (i % 8 != 0)                        // Not on the left edge
            attacks |= bitboardWithPiece >> 1; // Left
        if (i < 56)                            // Not on the top edge
            attacks |= bitboardWithPiece << 8; // Up
        if (i > 7)                             // Not on the bottom edge
            attacks |= bitboardWithPiece >> 8; // Down
        if (i % 8 != 7 && i < 56)              // Not on the right edge and not on the top edge
            attacks |= bitboardWithPiece << 9; // Up-Right
        if (i % 8 != 0 && i < 56)              // Not on the left edge and not on the top edge
            attacks |= bitboardWithPiece << 7; // Up-Left
        if (i % 8 != 7 && i > 7)               // Not on the right edge and not on the bottom edge
            attacks |= bitboardWithPiece >> 7; // Down-Right
        if (i % 8 != 0 && i > 7)               // Not on the left edge and not on the bottom edge
            attacks |= bitboardWithPiece >> 9; // Down-Left

        kingPseudoAttacks[i] = attacks;
    }
    return;
}

int ChessGame::pop_lsb(uint64_t &bb) const
{
    int index = __builtin_ctzll(bb); // Count trailing zeros
    bb &= bb - 1;                    // Clear the least significant bit
    return index;
}

int ChessGame::pop_msb(uint64_t &bb) const
{
    int index = 63 - __builtin_clzll(bb); // Count leading zeros
    bb ^= (1ULL << index);                // Clear the most significant bit (we already know it's 1)
    return index;
}

void ChessGame::printBitboard(uint64_t bitboard) const
{
    for (int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < 8; j++)
        {
            if (bitboard & (1ULL << (i * 8 + j)))
            {
                std::cout << "1 ";
            }
            else
            {
                std::cout << "0 ";
            }
        }
        std::cout << std::endl;
    }
}

void ChessGame::initializeRayAttacks()
{
    // Initialize ray attacks table to use as lookup for sliding pieces
    uint64_t northMask = 0x0101010101010100;
    for (int sq = 0; sq < 64; ++sq)
    {
        rayAttacks[sq][static_cast<int>(Direction::North)] = northMask;
        northMask <<= 1;
    }
    uint64_t eastMask = 0x00000000000000FE;
    for (int file = 0; file < 8; ++file)
    {
        uint64_t eastMaskCopy = eastMask;
        for (int rank = 0; rank < 8; ++rank)
        {
            int sq = rank * 8 + file;
            rayAttacks[sq][static_cast<int>(Direction::East)] = eastMaskCopy;
            eastMaskCopy <<= 8;
        }
        pop_msb(eastMask); // Clear the most significant bit
        eastMask <<= 1;    // Shift left to prepare for the next file
    }
    uint64_t northEastMask = 0x8040201008040200;
    for (int file = 0; file < 8; ++file)
    {
        uint64_t northEastMaskCopy = northEastMask;
        for (int rank = 0; rank < 8; ++rank)
        {
            int sq = rank * 8 + file;
            rayAttacks[sq][static_cast<int>(Direction::NorthEast)] = northEastMaskCopy;
            northEastMaskCopy <<= 8;
        }
        pop_msb(northEastMask); // Clear the most significant bit
        northEastMask <<= 1;    // Shift left to prepare for the next file
    }

    uint64_t southMask = 0x0080808080808080;
    for (int sq = 63; sq >= 0; --sq)
    {
        rayAttacks[sq][static_cast<int>(Direction::South)] = southMask;
        southMask >>= 1;
    }

    uint64_t westMask = 0x7F00000000000000;
    for (int file = 7; file >= 0; --file)
    {
        uint64_t westMaskCopy = westMask;
        for (int rank = 7; rank >= 0; --rank)
        {
            int sq = rank * 8 + file;
            rayAttacks[sq][static_cast<int>(Direction::West)] = westMaskCopy;
            westMaskCopy >>= 8;
        }
        pop_lsb(westMask); // Clear the most significant bit
        westMask >>= 1;    // Shift right to prepare for the next file
    }

    uint64_t southWestMask = 0x0040201008040201;
    for (int file = 7; file >= 0; --file)
    {
        uint64_t southWestMaskCopy = southWestMask;
        for (int rank = 7; rank >= 0; --rank)
        {
            int sq = rank * 8 + file;
            rayAttacks[sq][static_cast<int>(Direction::SouthWest)] = southWestMaskCopy;
            southWestMaskCopy >>= 8;
        }
        pop_lsb(southWestMask); // Clear the most significant bit
        southWestMask >>= 1;    // Shift right to prepare for the next file
    }

    uint64_t northWestMask = 0x0102040810204000;
    for (int file = 7; file >= 0; --file)
    {
        uint64_t northWestMaskCopy = northWestMask;
        for (int rank = 0; rank < 8; ++rank)
        {
            int sq = rank * 8 + file;
            rayAttacks[sq][static_cast<int>(Direction::NorthWest)] = northWestMaskCopy;
            northWestMaskCopy <<= 8;
        }
        pop_msb(northWestMask); // Clear the most significant bit
        northWestMask >>= 1;    // Shift left to prepare for the next file
    }

    uint64_t southEastMask = 0x0002040810204080;
    for (int file = 0; file < 8; ++file)
    {
        uint64_t southEastMaskCopy = southEastMask;
        for (int rank = 7; rank >= 0; --rank)
        {
            int sq = rank * 8 + file;
            rayAttacks[sq][static_cast<int>(Direction::SouthEast)] = southEastMaskCopy;
            southEastMaskCopy >>= 8;
        }
        pop_lsb(southEastMask); // Clear the most significant bit
        southEastMask <<= 1;    // Shift right to prepare for the next file
    }
    return;
}

uint64_t ChessGame::getPositiveRayAttacks(uint64_t occupied, Direction dir8, unsigned long square) const
{
    uint64_t attacks = rayAttacks[square][static_cast<int>(dir8)];
    uint64_t blocker = attacks & occupied;
    int firstBlockingSquare = blocker ? __builtin_ctzll(blocker) : 0;
    attacks ^= rayAttacks[firstBlockingSquare][static_cast<int>(dir8)];
    // std::cout << "Positive ray attacks for square " << square << " in direction " << //static_cast<int>(dir8) << ": " << std::endl;
    // printBitboard(attacks);
    // std::cout << "-------------------------" << std::endl;
    return attacks;
}

uint64_t ChessGame::getNegativeRayAttacks(uint64_t occupied, Direction dir8, unsigned long square) const
{
    uint64_t attacks = rayAttacks[square][static_cast<int>(dir8)];
    uint64_t blocker = attacks & occupied;
    int firstBlockingSquare = blocker ? (63 - __builtin_clzll(blocker)) : 0;
    attacks ^= rayAttacks[firstBlockingSquare][static_cast<int>(dir8)];
    // std::cout << "Negative ray attacks for square " << square << " in direction " << static_cast<int>(dir8) << ": " << std::endl;
    // printBitboard(attacks);
    // std::cout << "-------------------------" << std::endl;
    return attacks;
}

uint64_t ChessGame::getBishopAttacks(uint64_t occupied, int sq) const
{
    return diagonalAttacks(occupied, sq) | antiDiagAttacks(occupied, sq);
}

uint64_t ChessGame::getRookAttacks(uint64_t occupied, int sq) const
{
    // printBitboard(fileAttacks(occupied, sq));
    // std::cout << "-------------------------" << std::endl;
    // printBitboard(rankAttacks(occupied, sq));
    // std::cout << "-------------------------" << std::endl;
    return fileAttacks(occupied, sq) | rankAttacks(occupied, sq);
}

uint64_t ChessGame::getQueenAttacks(uint64_t occupied, int sq) const
{
    return getRookAttacks(occupied, sq) | getBishopAttacks(occupied, sq);
}

int ChessGame::parseSquare(const std::string &square)
{
    if (square.length() != 2)
        return -1; // Invalid square

    int file = square[0] - 'a';
    int rank = square[1] - '1';

    if (file < 0 || file > 7 || rank < 0 || rank > 7)
        return -1; // Invalid square

    return rank * 8 + file; // Convert to single index
}

void ChessGame::printBoardWithMovesByPiece(Square square) const
{
    for (int i = 0; i < 12; ++i)
    {
        if (pieceBitboards[i] & (1ULL << static_cast<int>(square)))
        {
            std::cout << "Piece " << i << " at square " << static_cast<int>(square) << " can move to: ";
            std::vector<Move> moves = generateMoves();
            for (const Move &move : moves)
            {
                if (move.from == square)
                {
                    std::cout << getSquareName(move.to) << " ";
                }
            }
            std::cout << std::endl;
        }
    }
}

std::string ChessGame::getSquareName(Square square)
{
    // Convert square index to algebraic notation (e.g., a1, h8)
    int file = static_cast<int>(square) % 8;
    int rank = static_cast<int>(square) / 8;

    return std::string(1, 'a' + file) + std::to_string(rank + 1);
}

bool ChessGame::isMoveLegal(const ChessGame::Move &move) const
{
    // Simulate the move and check if it results in a legal position
}

void ChessGame::preworkPosition()
{
    whitePieces = pieceBitboards[0] | pieceBitboards[1] | pieceBitboards[2] | pieceBitboards[3] | pieceBitboards[4] | pieceBitboards[5];
    blackPieces = pieceBitboards[6] | pieceBitboards[7] | pieceBitboards[8] | pieceBitboards[9] | pieceBitboards[10] | pieceBitboards[11];
    occupiedBitboard = whitePieces | blackPieces;
    emptyBitboard = ~occupiedBitboard;
    // Precompute knight and king attacks
    initializeKnightAttacks();
    initializeKingAttacks();

    // Initialize ray attacks for sliding pieces
    initializeRayAttacks();

    // Reset the board to the initial position
    pinInfoStruct = calculatePins(occupiedBitboard, emptyBitboard, whiteTurn);
    std::cout << "Pins calculated: " << std::endl;
    for (int i = 0; i < 64; ++i)
    {
        if (pinInfoStruct.pinned_pieces & (1ULL << i))
        {
            std::cout << "Pinned piece at square: " << getSquareName(static_cast<Square>(i)) << std::endl;
            std::cout << "Pin ray: ";
            printBitboard(pinInfoStruct.pin_rays[i]);
        }
    }

    checkInfoStruct = calculateCheckInfo();
    std::cout << "Check Info: " << std::endl;
    std::cout << "Is in check: " << (checkInfoStruct.isInCheck ? "Yes" : "No") << std::endl;
    if (checkInfoStruct.isInCheck)
    {
        std::cout << "Checkers: " << std::endl;
        printBitboard(checkInfoStruct.checkers);
        std::cout << "Check ray: " << std::endl;
        printBitboard(checkInfoStruct.checkBlockSquares);
        std::cout << std::endl;
    }
    else
    {
        std::cout << "No check detected." << std::endl;
    }

    return;
}

PinInfo ChessGame::calculatePins(uint64_t our_pieces, uint64_t enemy_pieces,
                                 bool is_white)
{
    PinInfo pins = {};

    int king_square = __builtin_ctzll(pieceBitboards[is_white ? 5 : 11]); // Get the king square

    // Check for rook/queen pins (orthogonal)
    uint64_t enemy_rooks_queens = is_white ? (pieceBitboards[7] | pieceBitboards[9] | pieceBitboards[10]) : // Black rooks | Black Bishops | Black queens
                                      (pieceBitboards[1] | pieceBitboards[3] | pieceBitboards[4]);          // White rooks | White bishops | White queens
    std::cout << "Enemy rooks/queens bitboard: " << std::endl;
    printBitboard(enemy_rooks_queens);
    std::cout << "--------------------------" << std::endl;
    // For each enemy rook/queen
    while (enemy_rooks_queens)
    {
        int enemy_square = pop_lsb(enemy_rooks_queens);

        // Get ray from enemy piece to king
        uint64_t ray = getRayBetween(enemy_square, king_square);
        if (!ray)
            continue; // Not on same rank/file
        std::cout << "Ray Between: " << std::endl;
        printBitboard(ray);
        std::cout << "--------------------------" << std::endl;
        // Count our pieces on this ray
        uint64_t our_pieces_on_ray = ray & our_pieces;
        if (__builtin_popcountll(our_pieces_on_ray) == 1)
        {
            // Exactly one piece - it's pinned!
            int pinned_square = __builtin_ctzll(our_pieces_on_ray);
            pins.pinned_pieces |= (1ULL << pinned_square);
            pins.pin_rays[pinned_square] = ray | (1ULL << enemy_square);
        }
    }

    return pins;
}

uint64_t ChessGame::getRayBetween(int from, int to) const
{
    // Get the ray between two squares if they're aligned
    // Calculate ray direction
    int diff = to - from;
    int fileDistance = (to % 8) - (from % 8);
    int rankDistance = (to / 8) - (from / 8);

    // Check if squares are aligned
    if (fileDistance != 0 && rankDistance != 0 &&
        std::abs(fileDistance) != std::abs(rankDistance))
        return 0; // Not on same diagonal or rank/file

    // Calculate direction
    int direction = 0;
    if (fileDistance > 0 && rankDistance == 0)
        direction = static_cast<int>(Direction::East); // East
    else if (fileDistance > 0 && rankDistance > 0)
        direction = static_cast<int>(Direction::NorthEast); // Northeast
    else if (fileDistance == 0 && rankDistance > 0)
        direction = static_cast<int>(Direction::North); // North
    else if (fileDistance < 0 && rankDistance > 0)
        direction = static_cast<int>(Direction::NorthWest); // Northwest
    else if (fileDistance < 0 && rankDistance == 0)
        direction = -static_cast<int>(Direction::West);
    // West
    else if (fileDistance < 0 && rankDistance < 0)
        direction = -static_cast<int>(Direction::SouthWest);
    // Southwest
    else if (fileDistance == 0 && rankDistance < 0)
        direction = -static_cast<int>(Direction::South);
    // South
    else if (fileDistance > 0 && rankDistance < 0)
        direction = -static_cast<int>(Direction::SouthEast);
    // Southeast

    uint64_t ray = 0;
    uint64_t toSquareBitboard = 1ULL << to;
    if (direction > 0) // TODO: make this more efficient
    {
        ray = getPositiveRayAttacks(toSquareBitboard, static_cast<Direction>(direction), from);
        pop_msb(ray);
    }
    else
    {
        direction = -direction; // Convert to positive direction for calculations
        ray = getNegativeRayAttacks(toSquareBitboard, static_cast<Direction>(direction), from);
        pop_lsb(ray); // Clear the least significant bit
    }
    // ray = (direction > 0) ? getPositiveRayAttacks(toSquareBitboard, static_cast<Direction>(direction), from)
    //                       : getNegativeRayAttacks(toSquareBitboard, static_cast<Direction>(-direction), from);

    return ray;
}

CheckInfo ChessGame::calculateCheckInfo()
{
    CheckInfo info = {};

    int kingSquare = __builtin_ctzll(pieceBitboards[whiteTurn ? 5 : 11]); // Get the king square
    uint64_t enemy_pieces = whiteTurn ? blackPieces : whitePieces;

    // Pawn attacks
    uint64_t pawnCheckMask = 0;
    if (whiteTurn)
    { // White king, check from black pawns
        // Check FROM northwest and northeast squares (black pawns above the king)
        if ((kingSquare % 8) > 0)                      // Not on a-file
            pawnCheckMask |= 1ULL << (kingSquare + 7); // Northwest attack
        if ((kingSquare % 8) < 7)                      // Not on h-file
            pawnCheckMask |= 1ULL << (kingSquare + 9); // Northeast attack

        // Check for black pawns (pieceBitboards[6])
        info.checkers |= pawnCheckMask & pieceBitboards[6];
    }
    else
    { // Black king, check from white pawns
        // Check FROM southwest and southeast squares (white pawns below the king)
        if ((kingSquare % 8) > 0)                      // Not on a-file
            pawnCheckMask |= 1ULL << (kingSquare - 9); // Southwest attack
        if ((kingSquare % 8) < 7)                      // Not on h-file
            pawnCheckMask |= 1ULL << (kingSquare - 7); // Southeast attack

        // Check for white pawns (pieceBitboards[0])
        info.checkers |= pawnCheckMask & pieceBitboards[0];
    }

    // Check for knight attacks
    uint64_t knight_attacks = knightPseudoAttacks[kingSquare];                    // Get knight attacks from king square
    uint64_t enemy_knights = (whiteTurn ? pieceBitboards[8] : pieceBitboards[2]); // Get enemy knights
    info.checkers |= knight_attacks & enemy_knights;

    // Check for sliding piece attacks (rooks, bishops, queens)
    // ... similar to pin detection but simpler
    // Get the rays in all directions from the king square
    uint64_t occupied = occupiedBitboard;
    // Check for enemy rooks/queens (orthogonal) and bishops/queens (diagonal)
    uint64_t enemy_sliding_pieces = (whiteTurn ? (pieceBitboards[7] | pieceBitboards[9] | pieceBitboards[10]) : // Black rooks/queens/bishops
                                         (pieceBitboards[1] | pieceBitboards[3] | pieceBitboards[4]));          // White rooks/queens/bishops
    for (int dir = static_cast<int>(Direction::NorthWest); dir <= static_cast<int>(Direction::West); ++dir)
    {
        uint64_t ray;
        if (dir < 4)
        { // Positive Rays
            ray = getPositiveRayAttacks(occupied, static_cast<Direction>(dir), kingSquare);
        }
        else
        {
            ray = getNegativeRayAttacks(occupied, static_cast<Direction>(dir), kingSquare);
        }
        if (ray == 0)
            continue; // No ray in this direction

        uint64_t sliding_checkers = ray & enemy_sliding_pieces;
        if (sliding_checkers)
        {
            info.checkers |= sliding_checkers;
        }
    }

    info.isInCheck = info.checkers != 0;

    // Calculate block squares if in single check
    if (info.isInCheck && __builtin_popcountll(info.checkers) == 1)
    {
        int checker_square = __builtin_ctzll(info.checkers);
        info.checkBlockSquares = getRayBetween(checker_square, kingSquare);
    }

    return info;
}