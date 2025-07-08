#include "Engine.h"
#include <climits>

class EnokiEngine : public Engine
{
public:
  EnokiEngine() = default;
  ~EnokiEngine() override = default;

  // Initialize the engine with the chessboard state
  void initialize(ChessGame *game) override
  {
    this->gamePtr = game; // Store the game state
  }

  // Evaluate the current position (dummy implementation)
  int evaluatePosition() const override
  {
    // Make a simple evaluation based on material count
    int score = 0;
    for (int i = 0; i < 12; ++i) // Loop through all piece types
    {
      uint64_t bitboard = this->gamePtr->getPieceBitboards()[i];
      int pieceCount = __builtin_popcountll(bitboard); // Count the number of pieces
      switch (i)
      {
      case 0: // Pawn
        score += pieceCount * 1;
        break;
      case 1: // Knight
        score += pieceCount * 3;
        break;
      case 2: // Bishop
        score += pieceCount * 3;
        break;
      case 3: // Rook
        score += pieceCount * 5;
        break;
      case 4: // Queen
        score += pieceCount * 9;
        break;
      case 5:                    // King
        score += pieceCount * 0; // King has no material value in evaluation
        break;
      case 6: // Black Pawn
        score -= pieceCount * 1;
        break;
      case 7: // Black Knight
        score -= pieceCount * 3;
        break;
      case 8: // Black Bishop
        score -= pieceCount * 3;
        break;
      case 9: // Black Rook
        score -= pieceCount * 5;
        break;
      case 10: // Black Queen
        score -= pieceCount * 9;
        break;
      case 11:                   // Black King
        score -= pieceCount * 0; // Black King has no material value in evaluation
        break;
      case 12: // Empty square
        break; // No score for empty squares
      default:
        break; // Ignore any other piece types
      }
    }
    return score; // Return the total score
  }

  // Get a random move from the current position
  ChessGame::Move getBestMove(int depth) override
  {
    srand(time(NULL));
    // Generate all possible moves and return a random one
    auto moves = this->gamePtr->generateMoves();
    if (moves.empty())
      return ChessGame::Move{}; // Return an empty move if no moves available
    if (this->gamePtr->isWhiteTurn())
    {
      // If it's white's turn, we want to maximize the score
      int bestScore = INT_MIN;
      ChessGame::Move bestMove;
      for (const auto &move : moves)
      {
        this->gamePtr->applyMove(move);
        int score = maxi(depth - 1);
        if (score > bestScore)
        {
          bestScore = score;
          bestMove = move;
        }
        this->gamePtr->undoMove(move);
      }
      return bestMove;
    }
    else
    {
      // If it's black's turn, we want to minimize the score
      int bestScore = INT_MAX;
      ChessGame::Move bestMove;
      for (const auto &move : moves)
      {
        this->gamePtr->applyMove(move);
        int score = mini(depth - 1);
        if (score < bestScore)
        {
          bestScore = score;
          bestMove = move;
        }
        this->gamePtr->undoMove(move);
      }
      return bestMove;
    }
  }

  int maxi(int depth)
  {
    if (depth <= 0)
      return evaluatePosition();
    int max = INT_MIN;
    int score;
    for (const auto &move : this->gamePtr->generateMoves())
    {
      this->gamePtr->applyMove(move);
      score = mini(depth - 1);
      this->gamePtr->undoMove(move);
      if (score > max)
        max = score;
    }
    return max;
  }

  int mini(int depth)
  {
    if (depth <= 0)
      return -evaluatePosition();
    int min = INT_MAX;
    int score;
    for (const auto &move : this->gamePtr->generateMoves())
    {
      this->gamePtr->applyMove(move);
      score = maxi(depth - 1);
      this->gamePtr->undoMove(move);
      if (score < min)
      {
        min = score;
      }
    }
    return min;
  }
};
