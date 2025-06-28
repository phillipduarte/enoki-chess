#include "Engine.h"

class RandomEngine : public Engine
{
public:
  RandomEngine() = default;
  ~RandomEngine() override = default;

  // Initialize the engine with the chessboard state
  void initialize(ChessGame *game) override
  {
    this->gamePtr = game; // Store the game state
  }

  // Evaluate the current position (dummy implementation)
  int evaluatePosition() const override
  {
    return 0; // Random engine does not evaluate positions
  }

  // Get a random move from the current position
  ChessGame::Move getBestMove(int depth) override
  {
    // Generate all possible moves and return a random one
    auto moves = this->gamePtr->generateMoves();
    if (moves.empty())
      return ChessGame::Move{}; // Return an empty move if no moves available

    size_t randomIndex = rand() % moves.size();
    return moves[randomIndex];
  }

private:
  ChessGame *gamePtr = nullptr; // Pointer to the chess game state
};