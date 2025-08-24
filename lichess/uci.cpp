// uci.cpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <ctime>

#include "/home/phild/enoki-chess/src/chess.h"           // Include your ChessGame header
#include "/home/phild/enoki-chess/src/engines/enoki.cpp" // Include the EnokiEngine header

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;

std::ofstream logFile;

// Function to write to log file
void log(const std::string &message)
{
  if (logFile.is_open())
  {
    // Add timestamp
    std::time_t now = std::time(nullptr);
    char timestamp[64];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    logFile << "[" << timestamp << "] " << message << std::endl;
    logFile.flush(); // Make sure it's written immediately
  }
}

static inline int sqIndexFromName(const string &s)
{
  // UCI squares are "e2", "e4", etc. Your ChessGame::parseSquare handles this:
  return ChessGame::parseSquare(s);
}

static inline string trim(const string &s)
{
  size_t a = s.find_first_not_of(" \t\r\n");
  if (a == string::npos)
    return "";
  size_t b = s.find_last_not_of(" \t\r\n");
  return s.substr(a, b - a + 1);
}

// Map promotion char to your Piece enum, which uses
// lower-case for WHITE pieces and upper-case for BLACK pieces (per chess.h).
static Piece promotionPieceFromChar(char promo, bool whiteToMove)
{
  promo = (char)std::tolower((unsigned char)promo);
  switch (promo)
  {
  case 'q':
    return whiteToMove ? Piece::q : Piece::Q;
  case 'r':
    return whiteToMove ? Piece::r : Piece::R;
  case 'b':
    return whiteToMove ? Piece::b : Piece::B;
  case 'n':
    return whiteToMove ? Piece::n : Piece::N;
  default:
    return Piece::e;
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  // Initialize log file
  logFile.open("/home/phild/enoki-chess/lichess/uci.log", std::ios::app);
  if (!logFile.is_open())
  {
    std::cerr << "Warning: Could not open log file" << std::endl;
  }
  log("UCI interface started");

  ChessGame game;
  EnokiEngine engine;
  engine.initialize(&game);

  // Emit initial ID/author as many GUIs call "uci" first.
  // We'll also respond again when "uci" is explicitly received.
  cout << "id name EnokiEngine\n";
  cout << "id author You\n";
  cout << "uciok\n";
  cout.flush();

  string line;
  while (getline(std::cin, line))
  {
    line = trim(line);
    if (line.empty())
      continue;

    if (line == "uci")
    {
      cout << "id name EnokiEngine\n";
      cout << "id author You\n";
      // If you expose runtime options, print `option name ...` lines here.
      cout << "uciok\n";
      cout.flush();
      continue;
    }

    if (line == "isready")
    {
      cout << "readyok\n";
      cout.flush();
      continue;
    }

    if (line.rfind("ucinewgame", 0) == 0)
    {
      game = ChessGame();       // reinitialize to starting position
      engine.initialize(&game); // rebind pointer
      continue;
    }

    if (line.rfind("position", 0) == 0)
    {
      // Syntax:
      //  position startpos [moves m1 m2 ...]
      //  position fen <FEN(6 fields)> [moves m1 m2 ...]
      istringstream ss(line);
      string tok;
      ss >> tok; // "position"

      string posType;
      ss >> posType;
      if (posType == "startpos")
      {
        game = ChessGame(); // reset to start
        engine.initialize(&game);

        // optional "moves"
        string maybeMoves;
        if (ss >> maybeMoves && maybeMoves == "moves")
        {
          string mv;
          while (ss >> mv)
          {
            bool ok = game.makeMove(mv);
            if (!ok)
            { /* ignore invalid */
            }
          }
        }
      }
      else if (posType == "fen")
      {
        log("Parsing FEN: " + posType);
        // FEN is 6 tokens
        string f1, f2, f3, f4, f5, f6;
        if (!(ss >> f1 >> f2 >> f3 >> f4 >> f5 >> f6))
        {
          // malformed FEN; ignore
        }
        else
        {
          string fen = f1 + " " + f2 + " " + f3 + " " + f4 + " " + f5 + " " + f6;
          log(fen);
          game.parseFEN(fen);
          // optional "moves"
          string maybeMoves;
          if (ss >> maybeMoves && maybeMoves == "moves")
          {
            string mv;
            while (ss >> mv)
            {

              bool ok = game.makeMove(mv);
              if (!ok)
              { /* ignore invalid */
              }
            }
          }
        }
      }
      continue;
    }

    if (line.rfind("go", 0) == 0)
    {
      // Supported:
      //   go depth N
      //   go wtime <ms> btime <ms> winc <ms> binc <ms>   (falls back to a depth guess)
      istringstream ss(line);
      string tok;
      ss >> tok; // "go"

      int depth = 4;
      int wtime = -1, btime = -1, winc = 0, binc = 0, movetime = -1;

      while (ss >> tok)
      {
        if (tok == "depth")
        {
          ss >> depth;
        }
        else if (tok == "wtime")
        {
          ss >> wtime;
        }
        else if (tok == "btime")
        {
          ss >> btime;
        }
        else if (tok == "winc")
        {
          ss >> winc;
        }
        else if (tok == "binc")
        {
          ss >> binc;
        }
        else if (tok == "movetime")
        {
          ss >> movetime;
        }
        // ignore others for now (nodes, mate, etc.)
      }

      if (depth <= 0)
      {
        // Very simple fallback: pick a small depth from the time controls.
        // You can replace this with a real time manager later.
        if (movetime > 0)
          depth = 4;
        else if (wtime >= 0 && btime >= 0)
          depth = 4;
        else
          depth = 3;
      }
      std::string fen = engine.getPtr()->generateFEN();
      log("Starting search for best move for FEN: " + fen);

      ChessGame::Move best = engine.getBestMove(depth);
      fen = engine.getPtr()->generateFEN();
      log("Best move for FEN: " + fen + " at depth " + std::to_string(depth) + ": " + ChessGame::moveToString(best));

      // Convert Move -> UCI string:
      auto squareName = [&](Square s)
      { return ChessGame::getSquareName(s); };
      string uci = squareName(best.from) + squareName(best.to);
      if (best.isPromotion && best.promotionPiece != Piece::e)
      {
        // UCI wants lowercase promo letter
        char c = 'q';
        switch (best.promotionPiece)
        {
        case Piece::q:
        case Piece::Q:
          c = 'q';
          break;
        case Piece::r:
        case Piece::R:
          c = 'r';
          break;
        case Piece::b:
        case Piece::B:
          c = 'b';
          break;
        case Piece::n:
        case Piece::N:
          c = 'n';
          break;
        default:
          break;
        }
        uci.push_back(c);
      }

      // cout << "bestmove " << uci << "\n";
      cout << "bestmove " << ChessGame::moveToString(best) << "\n";
      cout.flush();
      continue;
    }

    if (line == "stop")
    {
      // If you add async search later, set a stop flag here.
      // For now, search is synchronous inside 'go'.
      continue;
    }

    if (line == "quit")
    {
      break;
    }

    // Also add before the return statement:
    logFile.close();

    // Unknown command: ignore gracefully.
  }

  return 0;
}
