#include <iostream>
#include <string>
#include <vector>

#include "./chess.h"

// Second argument is the game mode; 0 -> local, 1 -> vs bot
// Third argument is the color; 0 -> white, 1 -> black; Only used in vs bot mode
int main(int argc, char* argv[]) {
    // Check if the first argument is provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <game_mode> [<color>]" << std::endl;
        return 1;
    }
    // Check if the first argument is a valid integer
    int game_mode = std::stoi(argv[1]);
    if (game_mode < 0 || game_mode > 1) {
        std::cerr << "Invalid game mode. Use 0 for local or 1 for vs bot." << std::endl;
        return 1;
    }
    // Check if the second argument is provided
    if (game_mode == 1 && argc < 3) {
        std::cerr << "Usage: " << argv[0] << " 1 <color>" << std::endl;
        return 1;
    }
    // Check if the color argument is valid
    if (game_mode == 1) {
        int color = std::stoi(argv[2]);
        if (color < 0 || color > 1) {
            std::cerr << "Invalid color. Use 0 for white or 1 for black." << std::endl;
            return 1;
        }
    }
    // If all checks pass, proceed with the game
    // Initialize game variables based on the provided arguments

    // Print a message indicating the game mode and color
    std::cout << "Game mode: " << (game_mode == 0 ? "Local" : "Vs Bot") << std::endl;    // (In a real game, you would initialize the game state here)
    if (game_mode == 1) {
        std::cout << "Color: " << (std::stoi(argv[2]) == 0 ? "White" : "Black") << std::endl;
    }




    return 0;
}