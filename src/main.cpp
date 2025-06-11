#include <iostream>
#include <string>
#include <vector>

#include "./chess.h"

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
    if (game_mode == 1)
    {
        int color = std::stoi(argv[2]);
        if (color < 0 || color > 1)
        {
            std::cerr << "Invalid color. Use 0 for white or 1 for black." << std::endl;
            return 1;
        }
    }
    // If all checks pass, proceed with the game
    // Initialize game variables based on the provided arguments

    // Print a message indicating the game mode and color
    std::cout << "Game mode: " << (game_mode == 0 ? "Local" : "Vs Bot") << std::endl; // (In a real game, you would initialize the game state here)
    if (game_mode == 1)
    {
        std::cout << "Color: " << (std::stoi(argv[2]) == 0 ? "White" : "Black") << std::endl;
    }

    const std::string defaultFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // Initialize the chess game
    ChessGame game;
    game.parseFEN(defaultFEN);
    game.printBoard(true);
    game.generateMoves();

    while (!game.isGameOver())
    {
        game.printBoard(false);
        std::string move;
        std::cout << "Current turn: " << (game.getCurrentTurn() == 0 ? "White" : "Black") << std::endl;
        if (game_mode == 1 && game.getCurrentTurn() == 1)
        {
            // Here you would implement the bot's move logic
            // For now, we will just simulate a bot move
            std::cout << "Bot is making a move..." << std::endl;
            // Simulate a random move or a predefined move
            move = "e2e4"; // Example of a bot move
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

        game.generateMoves();

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

    return 0;
}