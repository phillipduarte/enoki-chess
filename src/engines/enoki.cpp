#include "Engine.h"
#include <climits>

class EnokiEngine : public Engine
{
public:
  EnokiEngine()
  { // Flip signs for the last 6 heatmaps (black piece heatmaps: indices 6..11)
    for (int i = 6; i < 12; ++i)
    {
      int *map = heatmaps[i];
      for (int sq = 0; sq < 64; ++sq)
      {
        map[sq] = -map[sq];
      }
    }
  }
  ~EnokiEngine() override = default;

  // Initialize the engine with the chessboard state
  void initialize(ChessGame *game) override
  {
    this->gamePtr = game; // Store the game state
  }

  int evalV2() const
  {
    // This function can be used for a more advanced evaluation if needed
    // Make a simple evaluation based on material count
    int score = 0;
    if (this->gamePtr->isGameOver())
    {
      if (this->gamePtr->getGameResult() == 1)
        return INT_MAX; // White wins
      else if (this->gamePtr->getGameResult() == -1)
        return INT_MIN; // Black wins
      else
        return 0; // Draw
    }
    for (int i = 0; i < 12; ++i) // Loop through all piece types
    {
      uint64_t bitboard = this->gamePtr->getPieceBitboards()[i];
      int pieceCount = __builtin_popcountll(bitboard); // Count the number of pieces
      switch (i)
      {
      case 0: // Pawn
        score += pieceCount * 100;
        break;
      case 1: // Knight
        score += pieceCount * 320;
        break;
      case 2: // Bishop
        score += pieceCount * 330;
        break;
      case 3: // Rook
        score += pieceCount * 500;
        break;
      case 4: // Queen
        score += pieceCount * 900;
        break;
      case 5:                    // King
        score += pieceCount * 0; // King has no material value in evaluation
        break;
      case 6: // Black Pawn
        score -= pieceCount * 100;
        break;
      case 7: // Black Knight
        score -= pieceCount * 320;
        break;
      case 8: // Black Bishop
        score -= pieceCount * 330;
        break;
      case 9: // Black Rook
        score -= pieceCount * 500;
        break;
      case 10: // Black Queen
        score -= pieceCount * 900;
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
    this->gamePtr->bitboardToBoardArray(); // Convert bitboards to board array for evaluation
    for (int i = 0; i < 64; ++i)
    {
      Square square = static_cast<Square>(i);
      Piece piece = this->gamePtr->getPieceAtSquareFromBB(square);
      if (piece != Piece::e)
      {
        int pieceIndex = static_cast<int>(piece) - 1; // Convert Piece enum to index (0-11)
        score += heatmaps[pieceIndex][i];
      }
    }

    // Let's start with mobility to break ties
    std::vector<ChessGame::Move> moves = this->gamePtr->getMovesVector();
    int mobility = moves.size() * 0.5;                            // Count the number of legal moves available
    score += this->gamePtr->isWhiteTurn() ? mobility : -mobility; // White wants to maximize mobility, Black wants to minimize it
    return score;                                                 // Return the total score
  }

  int evaluatePosition() const override
  {
    // Make a simple evaluation based on material count
    int score = 0;
    if (this->gamePtr->isGameOver())
    {
      if (this->gamePtr->getGameResult() == 1)
        return INT_MAX; // White wins
      else if (this->gamePtr->getGameResult() == -1)
        return INT_MIN; // Black wins
      else
        return 0; // Draw
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
      return evalV2();
    // return evaluatePosition();

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
      return evalV2();
    // return evaluatePosition();

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

private:
  // Add any private members or methods if needed

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

  int white_pawn_heatmap[64] = {
      0, 0, 0, 0, 0, 0, 0, 0,
      5, 10, 10, -20, -20, 10, 10, 5,
      5, -5, -10, 0, 0, -10, -5, 5,
      0, 0, 0, 20, 20, 0, 0, 0,
      5, 5, 10, 25, 25, 10, 5, 5,
      10, 10, 20, 30, 30, 20, 10, 10,
      50, 50, 50, 50, 50, 50, 50, 50,
      0, 0, 0, 0, 0, 0, 0, 0};

  int black_pawn_heatmap[64] = {
      0, 0, 0, 0, 0, 0, 0, 0,
      50, 50, 50, 50, 50, 50, 50, 50,
      10, 10, 20, 30, 30, 20, 10, 10,
      5, 5, 10, 25, 25, 10, 5, 5,
      0, 0, 0, 20, 20, 0, 0, 0,
      5, -5, -10, 0, 0, -10, -5, 5,
      5, 10, 10, -20, -20, 10, 10, 5,
      0, 0, 0, 0, 0, 0, 0, 0};

  int white_knight_heatmap[64] = {
      -50, -40, -30, -30, -30, -30, -40, -50,
      -40, -20, 0, 5, 5, 0, -20, -40,
      -30, 5, 10, 15, 15, 10, 5, -30,
      -30, 0, 15, 20, 20, 15, 0, -30,
      -30, 5, 15, 20, 20, 15, 5, -30,
      -30, 0, 10, 15, 15, 10, 0, -30,
      -40, -20, 0, 0, 0, 0, -20, -40,
      -50, -40, -30, -30, -30, -30, -40, -50};

  int black_knight_heatmap[64] = {
      -50, -40, -30, -30, -30, -30, -40, -50,
      -40, -20, 0, 0, 0, 0, -20, -40,
      -30, 0, 10, 15, 15, 10, 0, -30,
      -30, 5, 15, 20, 20, 15, 5, -30,
      -30, 0, 15, 20, 20, 15, 0, -30,
      -30, 5, 10, 15, 15, 10, 5, -30,
      -40, -20, 0, 5, 5, 0, -20, -40,
      -50, -40, -30, -30, -30, -30, -40, -50};

  int white_bishop_heatmap[64] = {
      -20, -10, -10, -10, -10, -10, -10, -20,
      -10, 5, 0, 0, 0, 0, 5, -10,
      -10, 10, 10, 10, 10, 10, 10, -10,
      -10, 0, 10, 10, 10, 10, 0, -10,
      -10, 5, 5, 10, 10, 5, 5, -10,
      -10, 0, 5, 10, 10, 5, 0, -10,
      -10, 0, 0, 0, 0, 0, 0, -10,
      -20, -10, -10, -10, -10, -10, -10, -20};

  int black_bishop_heatmap[64] = {
      -20, -10, -10, -10, -10, -10, -10, -20,
      -10, 0, 0, 0, 0, 0, 0, -10,
      -10, 0, 5, 10, 10, 5, 0, -10,
      -10, 5, 5, 10, 10, 5, 5, -10,
      -10, 0, 10, 10, 10, 10, 0, -10,
      -10, 10, 10, 10, 10, 10, 10, -10,
      -10, 5, 0, 0, 0, 0, 5, -10,
      -20, -10, -10, -10, -10, -10, -10, -20};

  int white_rook_heatmap[64] = {
      0, 0, 0, 5, 5, 0, 0, 0,
      -5, 0, 0, 0, 0, 0, 0, -5,
      -5, 0, 0, 0, 0, 0, 0, -5,
      -5, 0, 0, 0, 0, 0, 0, -5,
      -5, 0, 0, 0, 0, 0, 0, -5,
      -5, 0, 0, 0, 0, 0, 0, -5,
      5, 10, 10, 10, 10, 10, 10, 5,
      0, 0, 0, 0, 0, 0, 0, 0};

  int black_rook_heatmap[64] = {
      0, 0, 0, 0, 0, 0, 0, 0,
      5, 10, 10, 10, 10, 10, 10, 5,
      -5, 0, 0, 0, 0, 0, 0, -5,
      -5, 0, 0, 0, 0, 0, 0, -5,
      -5, 0, 0, 0, 0, 0, 0, -5,
      -5, 0, 0, 0, 0, 0, 0, -5,
      -5, 0, 0, 0, 0, 0, 0, -5,
      0, 0, 0, 5, 5, 0, 0, 0};

  int white_queen_heatmap[64] = {
      -20, -10, -10, -5, -5, -10, -10, -20,
      -10, 0, 5, 0, 0, 0, 0, -10,
      -10, 5, 5, 5, 5, 5, 0, -10,
      0, 0, 5, 5, 5, 5, 0, -5,
      -5, 0, 5, 5, 5, 5, 0, -5,
      -10, 0, 5, 5, 5, 5, 0, -10,
      -10, 0, 0, 0, 0, 0, 0, -10,
      -20, -10, -10, -5, -5, -10, -10, -20};

  int black_queen_heatmap[64] = {
      -20, -10, -10, -5, -5, -10, -10, -20,
      -10, 0, 0, 0, 0, 0, 0, -10,
      -10, 0, 5, 5, 5, 5, 0, -10,
      -5, 0, 5, 5, 5, 5, 0, -5,
      0, 0, 5, 5, 5, 5, 0, -5,
      -10, 5, 5, 5, 5, 5, 0, -10,
      -10, 0, 5, 0, 0, 0, 0, -10,
      -20, -10, -10, -5, -5, -10, -10, -20};

  int white_king_heatmap[64] = {
      20, 30, 10, 0, 0, 10, 30, 20,
      20, 20, 0, 0, 0, 0, 20, 20,
      -10, -20, -20, -20, -20, -20, -20, -10,
      -20, -30, -30, -40, -40, -30, -30, -20,
      -30, -40, -40, -50, -50, -40, -40, -30,
      -30, -40, -40, -50, -50, -40, -40, -30,
      -30, -40, -40, -50, -50, -40, -40, -30,
      -30, -40, -40, -50, -50, -40, -40, -30};

  int black_king_heatmap[64] = {
      -30, -40, -40, -50, -50, -40, -40, -30,
      -30, -40, -40, -50, -50, -40, -40, -30,
      -30, -40, -40, -50, -50, -40, -40, -30,
      -30, -40, -40, -50, -50, -40, -40, -30,
      -20, -30, -30, -40, -40, -30, -30, -20,
      -10, -20, -20, -20, -20, -20, -20, -10,
      20, 20, 0, 0, 0, 0, 20, 20,
      20, 30, 10, 0, 0, 10, 30, 20};

  // Starting with white pieces than black -> prnbqkPRNBQK
  int *heatmaps[12] = {
      white_pawn_heatmap,
      white_rook_heatmap,
      white_knight_heatmap,
      white_bishop_heatmap,
      white_queen_heatmap,
      white_king_heatmap,
      black_pawn_heatmap,
      black_rook_heatmap,
      black_knight_heatmap,
      black_bishop_heatmap,
      black_queen_heatmap,
      black_king_heatmap};
};
