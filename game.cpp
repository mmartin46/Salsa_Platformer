#include "game.hpp"

GameState::GameState()
{
   this->set_time(0);
   this->set_scrollX(0);
   this->set_scrollY(0);
   this->init_blocks();
}

void GameState::process()
{
    // add time
    this->set_time(this->get_time() + 1);

    // plyr movement
    Player *plyr = &this->plyr;
    plyr->set_x(plyr->get_x() + plyr->get_dx());
    plyr->set_y(plyr->get_y() + plyr->get_dy());

    if (plyr->get_dx() != 0 && plyr->get_onBlock() && (plyr->get_slowingDown() == false) )
    {
        if (this->get_time() % 8 == 0)
        {
            if (plyr->get_animFrame() == 0)
            {
                plyr->set_animFrame(1);
            }
            else
            {
                plyr->set_animFrame(0);
            }
        }
    }

    plyr->apply_gravity();

    this->set_scrollX(-this->plyr.get_x() + 320);
    this->set_scrollY(-this->plyr.get_y() + 320);

    if (this->get_scrollX() > 0)
    {
        this->set_scrollX(0);
    }
    if (this->get_scrollX() < -38000+320)
    {
        this->set_scrollX(-38000+320);
    }
}


void GameState::collisionDetect()
{
    // Check for collision with any blocks (brick blocks)
    for (int i = 0; i < MAP_ROWS; i++)
    {
      for (int j = 0; j < MAP_COLUMNS; j++)
      {
        float pw = PLAYER_WIDTH, ph = PLAYER_HEIGHT;
        float px = this->plyr.get_x(), py = this->plyr.get_y();
        float bx = this->tile[i][j].get_x(), by = this->tile[i][j].get_y(), bw = this->tile[i][j].get_w(), bh = this->tile[i][j].get_h();
    
        if (px+pw/2 > bx && px+pw/2 < bx+bw)
        {
            // Head Bump
            if (py < by+bh && py>by && this->plyr.get_dy() < 0)
            {
                // correct y
                this->plyr.set_y(by+bh);
                py = by+bh;

                // bumped our head, stop any jump velocity
                this->plyr.set_dy(0);
                this->plyr.set_onBlock();
            }
        }
        if (px+pw > bx && px<bx+bw)
        {
            // Head bump
            if (py+ph > by && py < by && this->plyr.get_dy() > 0)
            {
                // correct y
                this->plyr.set_y(by-ph);
                py = by-ph;

                //landed on this ledge, stop any jump velocity
                this->plyr.set_dy(0);
                this->plyr.set_onBlock();
            }
        }
        if (py+ph > by && py<by+bh)
        {
            // Rubbing against right edge
            if (px < bx+bw && px+pw > bx+bw && this->plyr.get_dx() < 0)
            {
                // correct x
                this->plyr.set_x(bx+bw);
                px = bx+bw;

                this->plyr.set_dx(0);
            }
            // Rubbing against left edge
            else if (px+pw > bx && px < bx && this->plyr.get_dx() > 0)
            {
                // correct x
                this->plyr.set_x(bx-pw);
                px = bx-pw;

                this->plyr.set_dx(0);
                std::cout << "LEFT EDGE\n";
            }
        }
      }
    }
}


int GameState::processEvents(SDL_Window *window)
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
                        if (this->plyr.get_onBlock())
                        {
                            this->plyr.set_dy(PLAYER_JUMP_HEIGHT);
                            this->plyr.reset_onBlock();
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
        this->plyr.apply_jump();
    }

    // Walking
    if (state[SDL_SCANCODE_LEFT])
    {
       this->plyr.move_left_x();
       if (this->plyr.get_dx() < -6)
       {
           this->plyr.move_left_dx(); 
       }
       this->plyr.set_facingLeft(true);
       this->plyr.set_slowingDown(false);
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
       this->plyr.move_right_x();;
       if (this->plyr.get_dx() > 6)
       {
           this->plyr.move_right_dx();
       }
       this->plyr.set_facingLeft(false);
       this->plyr.set_slowingDown(false);
    }
    else
    {
        // Slows down to 0.
        this->plyr.set_animFrame(0);
        this->plyr.slow_movement();
        this->plyr.set_slowingDown(true);
        if (SDL_fabsf(this->plyr.get_dx()) < 0.1f)
        {
            this->plyr.apply_static_movement();
        }
    }
    return done;
}


// Load images and create rending textures from the images
void GameState::loadImages()
{
    SDL_Surface *surface = NULL;
    
    surface = IMG_Load("img\\plyr_ita.png");
    if (surface == NULL)
    {
        printf("Cannot find plyr_ita.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    this->plyrFrames[0] = SDL_CreateTextureFromSurface(this->get_renderer(), surface);
    SDL_FreeSurface(surface);

    // Loading the image's second frame.
    surface = IMG_Load("img\\plyr_itb.png");
    if (surface == NULL)
    {
        printf("Cannot find plyr_itb.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    this->plyrFrames[1] = SDL_CreateTextureFromSurface(this->get_renderer(), surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("img\\block.png");
    if (surface == NULL)
    {
        printf("Cannot find block.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    this->set_block(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
    SDL_FreeSurface(surface);

}

void GameState::loadGame()
{
    const char *font_name = "fonts\\ka1.ttf";
    loadImages();
}

void GameState::init_blocks()
{
    int x, y;

    for (x = 0; x < MAP_ROWS; ++x)
    {
        for (y = 0; y < MAP_COLUMNS; ++y)
        {
            tilemap[x][y] = world_map::map[x][y];
        }
    }
}

void GameState::doRender(SDL_Renderer *renderer)
{
    // set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // clear the screen (to blue)
    SDL_RenderClear(renderer);

    // set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Intialize the map
    int x, y; 
    for (x = 0; x < MAP_ROWS; ++x)
    {
        for (y = 0; y < MAP_COLUMNS; ++y)
        {
            tile[x][y].set_x(x*BLOCK_HEIGHT);
            tile[x][y].set_y(y*BLOCK_WIDTH);
            tile[x][y].set_w(BLOCK_WIDTH);
            tile[x][y].set_h(BLOCK_HEIGHT);

            //std::cout << tilemap[x][y];
        }
    }

    for (x = 0; x < MAP_ROWS; ++x)
    {
        for (y = 0; y < MAP_COLUMNS; ++y)
        {
            SDL_Rect blockRect = { static_cast<int>(this->get_scrollX() + tile[x][y].get_x()), static_cast<int>(this->get_scrollY() + tile[x][y].get_y()), tile[x][y].get_w(), tile[x][y].get_h() };

            switch (tilemap[x][y])
            {
                case 0:
                    SDL_RenderCopy(this->get_renderer(), this->get_block(), NULL , &blockRect);
                    break;
            }
        }
    }

    // draw a rectangle at plyr's position
    SDL_Rect rect = {  static_cast<int>(this->get_scrollX() + this->plyr.get_x()), static_cast<int>(this->get_scrollY() + this->plyr.get_y()), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopyEx(renderer, this->plyrFrames[this->plyr.get_animFrame()], NULL, &rect, 0, NULL, (SDL_RendererFlip)(this->plyr.get_facingLeft() == 0));


    SDL_RenderPresent(renderer);    
}