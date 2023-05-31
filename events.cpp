#include "main.hpp"


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
       gameState->plyr.set_facingLeft(true);
       gameState->plyr.set_slowingDown(false);
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
       gameState->plyr.move_right_x();;
       if (gameState->plyr.get_dx() > 6)
       {
           gameState->plyr.move_right_dx();
       }
       gameState->plyr.set_facingLeft(false);
       gameState->plyr.set_slowingDown(false);
    }
    else
    {
        // Slows down to 0.
        gameState->plyr.set_animFrame(0);
        gameState->plyr.slow_movement();
        gameState->plyr.set_slowingDown(true);
        if (SDL_fabsf(gameState->plyr.get_dx()) < 0.1f)
        {
            gameState->plyr.apply_static_movement();
        }
    }
    return done;
}