// Local headers

// SDL headers
#include <SDL2/SDL.h>

// C++ headers
#include <iostream>

int main(int argc, char *argv[]) {
    SDL_Window * window = SDL_CreateWindow("TicTacToe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_ALLOW_HIGHDPI);

    std::cout << "Hi\n";

    return 0;
}