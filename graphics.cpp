#include "main.hpp"
#include "player.hpp"

// Load images and create rending textures from the images
void loadImages(GameState *gameState)
{
    SDL_Surface *surface = NULL;
    
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
    gameState->set_block(SDL_CreateTextureFromSurface(gameState->get_renderer(), surface));
    SDL_FreeSurface(surface);

}

void loadGame(GameState *gameState)
{
    const char *font_name = "fonts\\ka1.ttf";
    loadImages(gameState);

    // initialize blocks
    typename std::vector<Block>::pointer ptr, end = gameState->blocks.data() + 100;
    uint8_t i = 0;
    for (ptr = gameState->blocks.data(); ptr < end; ++ptr)
    {
        ptr->set_w(60);
        ptr->set_h(20);
        ptr->set_x(i*60);
        ptr->set_y(400);
        ++i;
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


    // Intialize the rectangle for the blocks.
    typename std::vector<Block>::pointer ptr, end = gameState->blocks.data() + 100;
    uint8_t i = 0;
    for (ptr = gameState->blocks.data(); ptr < end; ++ptr)
    {
        SDL_Rect blockRect = { static_cast<int>(gameState->get_scrollX() + ptr->get_x()), ptr->get_y(), ptr->get_w(), ptr->get_h() };
        SDL_RenderCopy(renderer, gameState->get_block(), NULL, &blockRect);
        ++i;
    }

    // draw a rectangle at plyr's position
    SDL_Rect rect = {  static_cast<int>(gameState->get_scrollX() + gameState->plyr.get_x()), static_cast<int>(gameState->plyr.get_y()), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopyEx(renderer, gameState->plyrFrames[gameState->plyr.get_animFrame()], NULL, &rect, 0, NULL, (SDL_RendererFlip)(gameState->plyr.get_facingLeft() == 0));


    SDL_RenderPresent(renderer);    
}
