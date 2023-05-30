#include "main.hpp"
#include "graphics.cpp"
#include "physics.cpp"
#include "events.cpp"

int main(int argc, char *argv[] )
{
    GameState gameState;
    SDL_Window *window = NULL;             // Declare a window
    SDL_Renderer *renderer = NULL;         // Declare a renderer
    SDL_Surface *starSurface = NULL;

    SDL_Init(SDL_INIT_VIDEO);       // Intialize SDL2
    srand((int) time(NULL));


    // Create an application window with the following settings:
    window = SDL_CreateWindow("Game Window",            // window title
                            SDL_WINDOWPOS_UNDEFINED,    // xpos
                            SDL_WINDOWPOS_UNDEFINED,    // ypos
                            640,                        // width
                            480,                        // height
                            0                           // flags
                            );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameState.renderer = renderer;

    loadGame(&gameState);

    // The window is open: ener program loop (see SDL_PollEvent)
    int done = 0;
    SDL_Event event;

    // Event loop
    while (!done)
    {

        std::cout << gameState.plyr.get_x() << " " << gameState.plyr.get_y() << std::endl;
        // Check for events
        done = processEvents(window, &gameState);

        process(&gameState);
        collisionDetect(&gameState);

        // Render display
        doRender(renderer, &gameState);

        // don't burn up the CPU
        // SDL_Delay(100);
    }

    // Shutdown game and upload all memory
    SDL_DestroyTexture(gameState.plyrFrames[0]);
    SDL_DestroyTexture(gameState.plyrFrames[1]);
    SDL_DestroyTexture(gameState.block);


    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    return 0;
}