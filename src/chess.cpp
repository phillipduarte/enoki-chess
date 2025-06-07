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

void ChessGame::printBoard() const
{
    // Print the chessboard to the console
    std::cout << "Chessboard:" << std::endl;
    for (int row = 0; row < 8; ++row)
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
    printBitboards();
}

bool ChessGame::makeMove(const std::string &move)
{
    if (!isValidMove(move))
    {
        return false;
    }
    applyMove(move);
    whiteTurn = !whiteTurn;
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
        uint64_t pMovesOne = (pieceBitboards[0] << 8) & emptyBitboard & ~rankConst[7];
        addMovesFromBitboard(moves, pMovesOne, 0, 8);

        // Pawn move by 2 square moves
        uint64_t pMovesTwo = (pieceBitboards[0] << 16) & emptyBitboard & (emptyBitboard << 8) & rankConst[3];
        addMovesFromBitboard(moves, pMovesTwo, 0, 16);

        // Pawn move captures to left
        uint64_t pMovesLeft = (pieceBitboards[0] << 9) & blackPieceBitboard & ~rankConst[7] & ~fileConst[7];
        addMovesFromBitboard(moves, pMovesLeft, 0, 9);
        // Pawn move captures to right
        uint64_t pMovesRight = (pieceBitboards[0] << 7) & blackPieceBitboard & ~rankConst[7] & ~fileConst[0];
        addMovesFromBitboard(moves, pMovesRight, 0, 7);

        // TODO: Pawn promotion moves

        // Knight moves
        uint64_t nCopy = pieceBitboards[2]; // Copy of knight bitboard
        int knightSq;
        while (nCopy)
        {
            knightSq = pop_lsb(nCopy); // Clear the least significant bit
            uint64_t knightAttacks = knightPseudoAttacks[knightSq] & ~whitePieceBitboard;

            while (knightAttacks)
            {
                int destSq = pop_lsb(knightAttacks); // Get the least significant bit (first attack)

                Move move;
                move.from = static_cast<Square>(knightSq);
                move.to = static_cast<Square>(destSq);
                move.isCapture = (blackPieceBitboard & (1ULL << destSq)) != 0;
                moves.push_back(move);
            }
        }

        // King Moves
        uint64_t kCopy = pieceBitboards[5]; // Copy of king bitboard
        int kingSq;
        while (kCopy)
        {
            kingSq = pop_lsb(kCopy); // Clear the least significant bit
            uint64_t kingAttacks = kingPseudoAttacks[kingSq] & ~whitePieceBitboard;

            while (kingAttacks)
            {
                int destSq = pop_lsb(kingAttacks); // Get the least significant bit (first attack)

                Move move;
                move.from = static_cast<Square>(kingSq);
                move.to = static_cast<Square>(destSq);
                move.isCapture = (blackPieceBitboard & (1ULL << destSq)) != 0;
                moves.push_back(move);
            }
        }
    }
    else
    {
        // Generate moves for black pieces
    }

    return moves;
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

    std::cout << "Ray attacks initialized." << std::endl;
    printBitboard(rayAttacks[36][static_cast<int>(Direction::NorthEast)]); // Example to print one ray attack
    std::cout << "-------------------------" << std::endl;
    printBitboard(rayAttacks[36][static_cast<int>(Direction::North)]); // Example to print one ray attack
    std::cout << "-------------------------" << std::endl;
    printBitboard(rayAttacks[36][static_cast<int>(Direction::East)]); // Example to print one ray attack
    std::cout << "-------------------------" << std::endl;
    printBitboard(rayAttacks[36][static_cast<int>(Direction::South)]); // Example to print one ray attack
    std::cout << "-------------------------" << std::endl;
    printBitboard(rayAttacks[36][static_cast<int>(Direction::West)]); // Example to print one ray attack
    std::cout << "-------------------------" << std::endl;
    printBitboard(rayAttacks[36][static_cast<int>(Direction::SouthWest)]); // Example to print one ray attack
    std::cout << "-------------------------" << std::endl;
    printBitboard(rayAttacks[36][static_cast<int>(Direction::NorthWest)]); // Example to print one ray attack
    std::cout << "-------------------------" << std::endl;
    printBitboard(rayAttacks[36][static_cast<int>(Direction::SouthEast)]); // Example to print one ray attack
    return;
}

uint64_t ChessGame::getRayAttacks(uint64_t occupied, Direction dir8, unsigned long square)
{
    uint64_t attacks = rayAttacks[square][static_cast<int>(dir8)];
    uint64_t blocker = attacks & occupied;
    int firstBlockingSquare = __builtin_ctzll(blocker);
    attacks ^= rayAttacks[firstBlockingSquare][static_cast<int>(dir8)];
    return attacks;
}

void ChessGame::initializeBishopAttacks() {};
void ChessGame::initializeRookAttacks() {};
void ChessGame::initializeQueenAttacks() {};
