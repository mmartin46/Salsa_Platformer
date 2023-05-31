#include "game.hpp"
#include "game.cpp"
#include "player.hpp"

// Are two rectangles colliding.
int collide2d(float x1, float y1, float x2, float y2, float ht1, float wt1, float wt2, float ht2)
{
    return (!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
}


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
    gameState.set_renderer(renderer);

    gameState.loadGame();

    // The window is open: ener program loop (see SDL_PollEvent)
    int done = 0;
    SDL_Event event;

    // Event loop
    while (!done)
    {

        // Displays the x and y position of the player
        std::cout << gameState.plyr.get_x() << " " << gameState.plyr.get_y() << std::endl;


        // Check for events
        done = gameState.processEvents(window);

        gameState.process();
        gameState.collisionDetect();

        // Render display
        gameState.doRender(renderer);

        // don't burn up the CPU
        // SDL_Delay(100);
    }

    // Shutdown game and upload all memory
    SDL_DestroyTexture(gameState.plyrFrames[0]);
    SDL_DestroyTexture(gameState.plyrFrames[1]);
    SDL_DestroyTexture(gameState.get_block());


    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    // Clean up
    SDL_Quit();
    return 0;
}