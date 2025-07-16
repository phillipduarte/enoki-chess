#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include "../chess.h" // Include your ChessGame header

class PerftTester
{
private:
  ChessGame &game;

public:
  PerftTester(ChessGame &chessGame) : game(chessGame) {}

  // Core perft function - counts leaf nodes at given depth
  uint64_t perft(int depth)
  {
    if (depth == 0)
    {
      return 1;
    }

    std::vector<ChessGame::Move> moves = game.generateMoves();
    if (depth == 1)
    {
      return moves.size(); // At depth 1, just count the moves
    }
    uint64_t nodes = 0;

    for (const auto &move : moves)
    {
      game.applyMove(move);
      nodes += perft(depth - 1);
      game.undoMove(move);
    }

    return nodes;
  }

  // Divide function - shows move breakdown at depth 1
  void divide(int depth)
  {
    if (depth <= 0)
    {
      std::cout << "Invalid depth for divide" << std::endl;
      return;
    }

    std::vector<ChessGame::Move> moves = game.generateMoves();
    uint64_t totalNodes = 0;

    std::cout << "\nDivide results for depth " << depth << ":" << std::endl;
    std::cout << "Move\t\tNodes" << std::endl;
    std::cout << "----\t\t-----" << std::endl;

    for (const auto &move : moves)
    {
      game.applyMove(move);
      uint64_t nodes = (depth == 1) ? 1 : perft(depth - 1);
      game.undoMove(move);

      // You might want to implement a toString() method for your Move struct
      // For now, this assumes you can output the move directly
      std::cout << "Move\t\t" << nodes << std::endl;
      totalNodes += nodes;
    }

    std::cout << "----\t\t-----" << std::endl;
    std::cout << "Total:\t\t" << totalNodes << std::endl;
  }

  // Timed perft test with results display
  void runPerftTest(int maxDepth)
  {
    std::cout << "Running Perft Test:" << std::endl;
    std::cout << "Depth\tNodes\t\tTime (ms)\tNPS" << std::endl;
    std::cout << "-----\t-----\t\t---------\t---" << std::endl;

    for (int depth = 1; depth <= maxDepth; depth++)
    {
      auto start = std::chrono::high_resolution_clock::now();
      uint64_t nodes = perft(depth);
      auto end = std::chrono::high_resolution_clock::now();

      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      double timeMs = duration.count();

      uint64_t nps = (timeMs > 0) ? (nodes * 1000) / timeMs : 0;

      std::cout << depth << "\t" << nodes << "\t\t"
                << std::fixed << std::setprecision(2) << timeMs << "\t\t"
                << nps << std::endl;
    }
  }

  // Verify against known perft results for starting position
  void verifyStartingPosition()
  {
    // Standard starting position perft results
    uint64_t expected[] = {
        0,        // depth 0 (not used)
        20,       // depth 1
        400,      // depth 2
        8902,     // depth 3
        197281,   // depth 4
        4865609,  // depth 5
        119060324 // depth 6
    };

    std::cout << "\nVerifying starting position perft results:" << std::endl;
    std::cout << "Depth\tExpected\tActual\t\tResult" << std::endl;
    std::cout << "-----\t--------\t------\t\t------" << std::endl;

    bool allCorrect = true;
    for (int depth = 1; depth <= 6; depth++)
    {
      uint64_t actual = perft(depth);
      bool correct = (actual == expected[depth]);
      allCorrect = allCorrect && correct;

      std::cout << depth << "\t" << expected[depth] << "\t\t"
                << actual << "\t\t" << (correct ? "PASS" : "FAIL") << std::endl;
    }

    std::cout << "\nOverall result: " << (allCorrect ? "PASS" : "FAIL") << std::endl;
  }
};

// Test function for a specific position
void testPosition(ChessGame &game, int depth)
{
  PerftTester tester(game);

  std::cout << "=== Perft Test ===" << std::endl;
  tester.runPerftTest(depth);

  std::cout << "\n=== Divide Test ===" << std::endl;
  tester.divide(depth);
}

// Test starting position
void testStartingPosition(ChessGame &game)
{
  PerftTester tester(game);

  std::cout << "=== Starting Position Verification ===" << std::endl;
  tester.verifyStartingPosition();
}

int main()
{
  try
  {
    // Create your chess game instance
    ChessGame game;                                                            // Assumes default constructor sets up starting position
    game.parseFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); // Reset to starting position
    game.preworkPosition();                                                    // Prepare the game state

    std::cout << "Chess Engine Perft Tester" << std::endl;
    std::cout << "=========================" << std::endl;

    // Test starting position
    testStartingPosition(game);

    // You can also test specific positions by setting up the game state
    // game.setPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // Run performance test
    std::cout << "\n=== Performance Test ===" << std::endl;
    testPosition(game, 6); // Test up to depth 5
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

// Compile with: g++ -std=c++11 -O3 -o perft perft.cpp ChessGame.cpp