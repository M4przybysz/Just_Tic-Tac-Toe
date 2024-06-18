// Local headers
#include "include/App.hpp"

// SDL headers

// C++ headers

int main(int argc, char *argv[]) {
    App& TicTacToe = App::getInstance();
    TicTacToe.init("TTT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 640, SDL_WINDOW_ALLOW_HIGHDPI);

    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;
    const double UPDATE_INTERVAL = 1.0 / 60;

    Uint32 frameStart;
    Uint32 lastUpdate = SDL_GetTicks(); 
    int frameTime;                     
    double deltaTime = 0.0;             
    double accumulator = 0.0;

    while(TicTacToe.isRunning()) {
        frameStart = SDL_GetTicks();

        TicTacToe.handleEvents();

        deltaTime = (SDL_GetTicks() - lastUpdate) / 1000.0;
        lastUpdate = SDL_GetTicks();
        accumulator += deltaTime;

        while(accumulator >= UPDATE_INTERVAL) {
            TicTacToe.update(UPDATE_INTERVAL);
            accumulator -= UPDATE_INTERVAL;   
        } 

        TicTacToe.render();

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) { SDL_Delay(FRAME_DELAY - frameTime); }
    }

    return EXIT_SUCCESS;
}