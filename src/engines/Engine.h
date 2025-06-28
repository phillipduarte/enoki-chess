#ifndef ENGINE_H
#define ENGINE_H

#include "../chess.h"
#include <cstdint>

class Engine
{
public:
  Engine() = default;
  virtual ~Engine() = default;

  // Initialize the engine with the chessboard state
  virtual void initialize(const ChessGame &game) = 0;

  // Generate moves for the current position
  // virtual std::vector<Move> generateMoves() = 0;

  // Evaluate the current position
  virtual int evaluatePosition() const = 0;

  // Make a move on the board
  // virtual void makeMove(const Move &move) = 0;

  // Undo the last move
  // virtual void undoMove() = 0;

  // Get the best move for the current position
  virtual ChessGame::Move getBestMove(int depth) = 0;
};

#endif // ENGINE_H