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
    if (this->gamePtr->isGameOver())
    {
      return this->gamePtr->isWhiteWins() ? INT_MAX : INT_MIN; // Return max/min score based on winner
    }
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
    // Add additional evaluation criteria here, such as piece positioning, control of the center, etc.
    // Let's start with mobility to break ties
    std::vector<ChessGame::Move> moves = this->gamePtr->getMovesVector();
    int mobility = moves.size() * 0.5; // Count the number of legal moves available
    if (this->gamePtr->isWhiteTurn())
    {
      score += mobility; // White wants to maximize mobility
    }
    else
    {
      score -= mobility; // Black wants to minimize mobility
    }
    return score; // Return the total score
  }

  // Get a random move from the current position
  ChessGame::Move getBestMove(int depth) override
  {
    srand(time(NULL));
    auto moves = this->gamePtr->generateMoves();
    if (moves.empty())
      return ChessGame::Move{};

    if (this->gamePtr->isWhiteTurn())
    {
      // White wants to maximize
      int bestScore = INT_MIN;
      ChessGame::Move bestMove;
      for (const auto &move : moves)
      {
        this->gamePtr->applyMove(move);
        int score = mini(depth - 1, bestScore, INT_MAX); // Alpha = bestScore, Beta = INT_MAX
        this->gamePtr->undoMove(move);

        if (score > bestScore)
        {
          bestScore = score;
          bestMove = move;
        }
      }
      return bestMove;
    }
    else
    {
      // Black wants to minimize
      int bestScore = INT_MAX;
      ChessGame::Move bestMove;
      for (const auto &move : moves)
      {
        this->gamePtr->applyMove(move);
        int score = maxi(depth - 1, INT_MIN, bestScore); // Alpha = INT_MIN, Beta = bestScore
        this->gamePtr->undoMove(move);

        if (score < bestScore)
        {
          bestScore = score;
          bestMove = move;
        }
      }
      return bestMove;
    }
  }

  int maxi(int depth, int alpha, int beta)
  {
    if (depth <= 0)
      return evaluatePosition();

    for (const auto &move : this->gamePtr->generateMoves())
    {
      this->gamePtr->applyMove(move);
      int score = mini(depth - 1, alpha, beta);
      this->gamePtr->undoMove(move);

      if (score > alpha)
        alpha = score;

      if (alpha >= beta)
        break; // Beta cutoff
    }
    return alpha;
  }

  int mini(int depth, int alpha, int beta)
  {
    if (depth <= 0)
      return evaluatePosition();

    for (const auto &move : this->gamePtr->generateMoves())
    {
      this->gamePtr->applyMove(move);
      int score = maxi(depth - 1, alpha, beta);
      this->gamePtr->undoMove(move);

      if (score < beta)
        beta = score;

      if (alpha >= beta)
        break; // Alpha cutoff
    }
    return beta;
  }
};
