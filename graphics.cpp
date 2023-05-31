#include "main.hpp"

void loadGame(GameState *gameState)
{
    SDL_Surface *surface = NULL;
    
    // Load images and create rending textures from them
    // Loading the image's first frame.
    surface = IMG_Load("img\\plyr_ita.png");
    if (surface == NULL)
    {
        printf("Cannot find plyr_ita.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    gameState->plyrFrames[0] = SDL_CreateTextureFromSurface(gameState->get_renderer(), surface);
    SDL_FreeSurface(surface);

    // Loading the image's second frame.
    surface = IMG_Load("img\\plyr_itb.png");
    if (surface == NULL)
    {
        printf("Cannot find plyr_itb.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    gameState->plyrFrames[1] = SDL_CreateTextureFromSurface(gameState->get_renderer(), surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("img\\block.png");
    if (surface == NULL)
    {
        printf("Cannot find block.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    gameState->block = SDL_CreateTextureFromSurface(gameState->get_renderer(), surface);
    SDL_FreeSurface(surface);



    gameState->plyr.set_x(210-PLAYER_WIDTH);    
    gameState->plyr.set_y(240-PLAYER_HEIGHT);
    gameState->plyr.set_dx(0);
    gameState->plyr.set_dy(0);
    gameState->plyr.reset_onBlock();
    gameState->plyr.animFrame = 0;
    gameState->plyr.facingLeft = 1;
    gameState->plyr.slowingDown = 0;

    gameState->set_time(0);
    gameState->set_scrollX(0);


    // init blocks
    for (int i = 0; i < 100; ++i)
    {
        gameState->blocks.at(i).w = 60;           
        gameState->blocks.at(i).h = 20;            
        gameState->blocks.at(i).x = i*60;         
        gameState->blocks.at(i).y = 400;                
    }
}



void doRender(SDL_Renderer *renderer, GameState *gameState)
{
    // set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // clear the screen (to blue)
    SDL_RenderClear(renderer);

    // set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 100; ++i)
    {
        SDL_Rect ledgeRect = { static_cast<int>(gameState->get_scrollX() + gameState->blocks[i].x), gameState->blocks[i].y, gameState->blocks[i].w, gameState->blocks[i].h };
        SDL_RenderCopy(renderer, gameState->block, NULL, &ledgeRect);
    }

    // draw a rectangle at plyr's position
    SDL_Rect rect = {  static_cast<int>(gameState->get_scrollX() + gameState->plyr.get_x()), static_cast<int>(gameState->plyr.get_y()), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopyEx(renderer, gameState->plyrFrames[gameState->plyr.animFrame], NULL, &rect, 0, NULL, (SDL_RendererFlip)(gameState->plyr.facingLeft == 0));


    SDL_RenderPresent(renderer);    
}
