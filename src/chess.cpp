#include "./chess.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

ChessGame::ChessGame() : whiteTurn(true)
{
  // Initialize the chessboard with pieces in their starting positions
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
        break;
      case 'n':
        piece = Piece::n;
        break;
      case 'b':
        piece = Piece::b;
        break;
      case 'q':
        piece = Piece::q;
        break;
      case 'k':
        piece = Piece::k;
        break;
      case 'P':
        piece = Piece::P;
        break;
      case 'R':
        piece = Piece::R;
        break;
      case 'N':
        piece = Piece::N;
        break;
      case 'B':
        piece = Piece::B;
        break;
      case 'Q':
        piece = Piece::Q;
        break;
      case 'K':
        piece = Piece::K;
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

  // These could be added as member variables if needed
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
  // Initialize the chessboard with pieces in their starting positions
}