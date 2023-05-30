#include "main.hpp"


void loadGame(GameState *gameState)
{
    SDL_Surface *surface = NULL;
    
    // Load images and create rending textures from them
    // Loading the image's first frame.
    surface = IMG_Load("plyr_ita.png");
    if (surface == NULL)
    {
        printf("Cannot find plyr_ita.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    gameState->plyrFrames[0] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
    SDL_FreeSurface(surface);

    // Loading the image's second frame.
    surface = IMG_Load("plyr_itb.png");
    if (surface == NULL)
    {
        printf("Cannot find plyr_itb.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    gameState->plyrFrames[1] = SDL_CreateTextureFromSurface(gameState->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("block.png");
    if (surface == NULL)
    {
        printf("Cannot find block.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    gameState->block = SDL_CreateTextureFromSurface(gameState->renderer, surface);
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
    gameState->scrollX = 0;


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
        SDL_Rect ledgeRect = { (int) (gameState->scrollX + gameState->blocks[i].x), gameState->blocks[i].y, gameState->blocks[i].w, gameState->blocks[i].h };
        SDL_RenderCopy(renderer, gameState->block, NULL, &ledgeRect);
    }

    // draw a rectangle at plyr's position
    SDL_Rect rect = { (int) (gameState->scrollX + gameState->plyr.get_x()), (int)gameState->plyr.get_y(), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopyEx(renderer, gameState->plyrFrames[gameState->plyr.animFrame], NULL, &rect, 0, NULL, (SDL_RendererFlip)(gameState->plyr.facingLeft == 0));


    SDL_RenderPresent(renderer);    
}

// Are two rectangles colliding.
int collide2d(float x1, float y1, float x2, float y2, float ht1, float wt1, float wt2, float ht2)
{
    return (!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
}

void process(GameState *gameState)
{
    // add time
    gameState->set_time(gameState->get_time() + 1);

    // plyr movement
    Player *plyr = &gameState->plyr;
    plyr->set_x(plyr->get_x() + plyr->get_dx());
    plyr->set_y(plyr->get_y() + plyr->get_dy());

    if (plyr->get_dx() != 0 && plyr->get_onBlock() && !plyr->slowingDown)
    {
        if (gameState->get_time() % 8 == 0)
        {
            if (plyr->animFrame == 0)
            {
                plyr->animFrame = 1;
            }
            else
            {
                plyr->animFrame = 0;
            }
        }
    }

    plyr->apply_gravity();

    gameState->scrollX = -gameState->plyr.get_x() + 320;
    
    if (gameState->scrollX > 0)
    {
        gameState->scrollX = 0;
    }
    if (gameState->scrollX < -38000+320)
    {
        gameState->scrollX = -38000+320;
    }
}


void collisionDetect(GameState *gameState)
{
    // Check for collision with any blocks (brick blocks)
    for (int i = 0; i < 100; i++)
    {
        float pw = PLAYER_WIDTH, ph = PLAYER_HEIGHT;
        float px = gameState->plyr.get_x(), py = gameState->plyr.get_y();
        float bx = gameState->blocks[i].x, by = gameState->blocks[i].y, bw = gameState->blocks[i].w, bh = gameState->blocks[i].h;
    
        if (px+pw/2 > bx && px+pw/2 < bx+bw)
        {
            // Head Bump
            if (py < by+bh && py>by && gameState->plyr.get_dy() < 0)
            {
                // correct y
                gameState->plyr.set_y(by+bh);
                py = by+bh;

                // bumped our head, stop any jump velocity
                gameState->plyr.set_dy(0);
                gameState->plyr.set_onBlock();
            }
        }
        if (px+pw > bx && px<bx+bw)
        {
            // Head bump
            if (py+ph > by && py < by && gameState->plyr.get_dy() > 0)
            {
                // correct y
                gameState->plyr.set_y(by-ph);
                py = by-ph;

                //landed on this ledge, stop any jump velocity
                gameState->plyr.set_dy(0);
                gameState->plyr.set_onBlock();
            }
        }
        if (py+ph > by && py<by+bh)
        {
            // Rubbing against right edge
            if (px < bx+bw && px+pw > bx+bw && gameState->plyr.get_dx() < 0)
            {
                // correct x
                gameState->plyr.set_x(bx+bw);
                px = bx+bw;

                gameState->plyr.set_dx(0);
            }
            // Rubbing against left edge
            else if (px+pw > bx && px < bx && gameState->plyr.get_dx() > 0)
            {
                // correct x
                gameState->plyr.set_x(bx-pw);
                px = bx-pw;

                gameState->plyr.set_dx(0);
            }
        }
    }
}




int processEvents(SDL_Window *window, GameState *gameState)
{
    SDL_Event event;
    int done = 0;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if (window)
                {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done = 1;
                }
            }
            break;
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        done = 1;
                    break;
                    case SDLK_UP:
                        if (gameState->plyr.get_onBlock())
                        {
                            gameState->plyr.set_dy(PLAYER_JUMP_HEIGHT);
                            gameState->plyr.reset_onBlock();
                        }
                }
            }
            break;
            case SDL_QUIT:
                // quit out of the game
                done = 1;
            break;
        }
    }

    // More jumping
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP])
    {
        gameState->plyr.apply_jump();
    }

    // Walking
    if (state[SDL_SCANCODE_LEFT])
    {
       gameState->plyr.move_left_x();
       if (gameState->plyr.get_dx() < -6)
       {
           gameState->plyr.move_left_dx(); 
       }
       gameState->plyr.facingLeft = 1;
       gameState->plyr.slowingDown = 0;
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
       gameState->plyr.move_right_x();;
       if (gameState->plyr.get_dx() > 6)
       {
           gameState->plyr.move_right_dx();
       }
       gameState->plyr.facingLeft = 0;
       gameState->plyr.slowingDown = 0;
    }
    else
    {
        // Slows down to 0.
        gameState->plyr.animFrame = 0;
        gameState->plyr.slow_movement();
        gameState->plyr.slowingDown = 1;
        if (SDL_fabsf(gameState->plyr.get_dx()) < 0.1f)
        {
            gameState->plyr.apply_static_movement();
        }
    }
    return done;
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