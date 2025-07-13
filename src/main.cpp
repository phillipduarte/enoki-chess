#include <iostream>
#include <string>
#include <vector>

#include "./chess.h"
#include "./engines/Engine.h"
#include "./engines/random.cpp"
#include "./engines/enoki.cpp"

// Second argument is the game mode; 0 -> local, 1 -> vs bot
// Third argument is the color; 0 -> white, 1 -> black; Only used in vs bot mode
int main(int argc, char *argv[])
{
    // Check if the first argument is provided
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <game_mode> [<color>]" << std::endl;
        return 1;
    }
    // Check if the first argument is a valid integer
    int game_mode = std::stoi(argv[1]);
    if (game_mode < 0 || game_mode > 1)
    {
        std::cerr << "Invalid game mode. Use 0 for local or 1 for vs bot." << std::endl;
        return 1;
    }
    // Check if the second argument is provided
    if (game_mode == 1 && argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " 1 <color>" << std::endl;
        return 1;
    }
    // Check if the color argument is valid
    int playerColor = 0; // Default to white
    if (game_mode == 1)
    {
        int playerColor = std::stoi(argv[2]);
        if (playerColor < 0 || playerColor > 1)
        {
            std::cerr << "Invalid color. Use 0 for white or 1 for black." << std::endl;
            return 1;
        }
    }
    // If all checks pass, proceed with the game
    // Initialize game variables based on the provided arguments

    // Print a message indicating the game mode and color
    std::cout << "Game mode: " << (game_mode == 0 ? "Local" : "Vs Bot") << std::endl;
    Engine *engine = nullptr;

    if (game_mode == 1)
    {
        std::cout << "Color: " << (playerColor) << std::endl;
        // Initialize the engine for bot play
        engine = new EnokiEngine();
        // You can replace RandomEngine with any other engine implementation
    }

    const std::string defaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const std::string testingFEN = "rnbq1bnr/pppppppp/4kq2/8/3B4/1P2P2Q/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    const std::string mateInOneFEN = "rnb1k1nr/pppp1ppp/5q2/2b1p3/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // Example FEN for a mate in one position
    const std::string pinBugFEN = "rnbqkbnr/pppp1ppp/8/4p3/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // Initialize the chess game
    ChessGame game;

    if (game_mode == 1)
    {
        engine->initialize(&game);
    }

    game.parseFEN(pinBugFEN); // Use the testing FEN for demonstration purposes
    // game.printBoard(true);
    game.preworkPosition();

    while (!game.isGameOver())
    {
        game.printBoard(false);
        std::string move;
        std::cout << "Current turn: " << (game.getCurrentTurn() == 0 ? "White" : "Black") << std::endl;
        if (game_mode == 1 && game.getCurrentTurn() != playerColor)
        {
            // Here you would implement the bot's move logic
            // For now, we will just simulate a bot move
            std::cout << "Bot is making a move..." << std::endl;
            // Simulate a random move or a predefined move
            ChessGame::Move moveStruct = engine->getBestMove(4); // Get the best move from the
            move = ChessGame::getSquareName(moveStruct.from) + ChessGame::getSquareName(moveStruct.to);
            game.generateMoves(); // Generate moves after the bot's move
        }
        else
        {
            // Get user input for the move
            std::cout << "Select your piece" << std::endl;
            std::string squareString;
            std::cin >> squareString;
            int sq = ChessGame::parseSquare(squareString);
            if (sq < 0 || sq > 63)
            {
                std::cout << "Invalid square. Please enter a number between 0 and 63." << std::endl;
                continue;
            }
            std::cout << "Selected square: " << squareString << std::endl;
            // game.printBoardWithMovesByPiece(static_cast<Square>(sq));
            std::cout << "Enter your move (or 'exit' to quit): ";
            std::string destSquareString;
            std::cin >> destSquareString;
            int destSq = ChessGame::parseSquare(destSquareString);
            if (destSq < 0 || destSq > 63)
            {
                std::cout << "Invalid destination square. Please enter a number between 0 and 63." << std::endl;
                continue;
            }
            std::cout << "Selected destination square: " << destSquareString << std::endl;
            // Construct the move string
            move = squareString + destSquareString;
            ChessGame::Move moveStruct;
            moveStruct.from = static_cast<Square>(sq);
            moveStruct.to = static_cast<Square>(destSq);
        }

        if (move == "exit")
        {
            break;
        }

        if (game.makeMove(move))
        {
            game.printBoard(false);
        }
        else
        {
            std::cout << "Invalid move. Try again." << std::endl;
        }
    }

    std::cout << "Game over!" << std::endl;
    return 0;
}