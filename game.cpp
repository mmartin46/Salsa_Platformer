#include "game.hpp"



/* Constructs the gamestate. */
GameState::GameState()
{
    this->set_time(0);
    this->set_scrollX(0);
    this->set_scrollY(0);
}

// Are two rectangles colliding.
int collide2d(float x1, float y1, float x2, float y2, float ht1, float wt1, float wt2, float ht2)
{
    return (!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
}

// Allows the user to load a texture and returns a surface.
//
// Parameters:
// \param file includes the file path
// \param error_msg includes an error message if the file isn't found.
SDL_Surface* get_surface(const char *file, const char *error_msg)
{
    SDL_Surface *surface = NULL;
    
    surface = IMG_Load(file);
    if (surface == NULL)
    {
        printf(error_msg);
        SDL_Quit();
        exit(1);
    } 
    return surface;   
}

void GameState::init_health_texture()
{
    char str[128] = "";
    sprintf(str, "Health: %u                      Tacos Eaten: %u", (int) this->get_life(), (int) this->get_tacos_eaten());

    SDL_Color white = { 255, 255, 255, 255 };

    SDL_Surface *tmp = TTF_RenderText_Blended(this->get_life_font(), str, white);
    this->life_label.set_w(tmp->w);
    this->life_label.set_h(tmp->h);
    this->set_life_label_texture(SDL_CreateTextureFromSurface(this->get_renderer(), tmp));
    SDL_FreeSurface(tmp);
}


// Load images and create rending textures from the images
void GameState::loadImages()
{

    // Load fonts
    set_life_font(TTF_OpenFont("img\\ka1.ttf", 48));
    if (!this->get_life_font())
    {
        printf("Cannot find font file!\n\n");
        SDL_Quit();
        exit(1);
    }
    set_taco_font(TTF_OpenFont("img\\ka1.ttf", 48));
    if (!this->get_taco_font())
    {
        printf("Cannot find font file!\n\n");
        SDL_Quit();
        exit(1);
    }

    // Load Sounds
    this->set_jump_music(Mix_LoadWAV("sounds\\jump.wav"));
    if (!this->get_jump_music())
    {
        printf("Cannot locate sounds\\jump.wav!");
        SDL_Quit();
        exit(1);
    }

    // Loading the player's first frame.
    SDL_Surface* surface = get_surface("img\\plyr_ita.png", "Cannot find plyr_ita.png!\n\n");
    this->plyrFrames[0] = SDL_CreateTextureFromSurface(this->get_renderer(), surface);
    SDL_FreeSurface(surface);

    // Loading the player's second frame.
    surface = get_surface("img\\plyr_itb.png", "Cannot find plyr_itb.png!\n\n");
    this->plyrFrames[1] = SDL_CreateTextureFromSurface(this->get_renderer(), surface);
    SDL_FreeSurface(surface);

    // Loading the block's texture.
    surface = get_surface("img\\block.png", "Cannot find block.png!\n\n");
    this->set_block(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
    SDL_FreeSurface(surface);

    // Loading the taco texture.
    surface = get_surface("img\\taco.png", "Cannot find block.png!\n\n");
    this->set_taco(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
    SDL_FreeSurface(surface);

    // Loading enemy texture.
    surface = get_surface("img\\enemy.png", "Cannot find enemy.png!\n\n");
    this->set_enemy(SDL_CreateTextureFromSurface(this->get_renderer(), surface));

    //Loading spike texture.
    surface = get_surface("img\\spike.png", "Cannot find spike.png!\n\n");
    this->set_spike(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
    SDL_FreeSurface(surface);

    // Loading soil texture.
    surface = get_surface("img\\soil.png", "Cannot find soil.png!\n\n");
    this->set_soil(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
    SDL_FreeSurface(surface);    

    // Loading background texture.
    if (this->get_level_choice() < 20)
    {
        surface = get_surface("img\\build_block.png", "Cannot find build_block.png!\n\n");
        this->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);    
    }
    else if (this->get_level_choice() >= 20)
    {
        // TODO: Change to the second level
        surface = get_surface("img\\build_block.png", "Cannot find build_block.png!\n\n");
        this->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);    
    }
}

void GameState::loadGame()
{
    const char *font_name = "fonts\\ka1.ttf";
    loadImages();
}


void GameState::init_blocks(int level_choice)
{
    
    int x, y;

    int choice = level_choice;
    set_level_choice(choice);
    if (choice < 20)
    {
        for (x = 0; x < MAP_ROWS; ++x)
        {
            for (y = 0; y < MAP_COLUMNS; ++y)
            {
                tilemap[x][y] = world_map::map[x][y];
            }
        }
    }
    else if (choice >= 20)
    {   
        for (x = 0; x < MAP_ROWS; ++x)
        {
            for (y = 0; y < MAP_COLUMNS; ++y)
            {
                tilemap[x][y] = world_map::map_2[x][y];
            }
        }
    }

    this->backdrop.set_x(-300);
    this->backdrop.set_y(-300);
    this->backdrop.set_h(5000);
    this->backdrop.set_w(5000);


    // Intialize the map
    for (x = 0; x < MAP_ROWS; ++x)
    {
        for (y = 0; y < MAP_COLUMNS; ++y)
        {
            // DEBUG
            switch (tilemap[x][y]) 
            {
                case world_map::BLOCK_COLLISION : {
                    tile[x][y].set_y((x*BLOCK_WIDTH) / 1);
                    tile[x][y].set_x((y*BLOCK_HEIGHT));
                    tile[x][y].set_w(BLOCK_WIDTH);
                    tile[x][y].set_h(BLOCK_HEIGHT);
                } break;
                case world_map::TACO_COLLISION : {
                    tile[x][y].set_y((x*BLOCK_WIDTH) / 1);
                    tile[x][y].set_x((y*BLOCK_HEIGHT));
                    tile[x][y].set_w(BLOCK_WIDTH);
                    tile[x][y].set_h(BLOCK_HEIGHT);
                } break;
                case world_map::EMEMY_COLLISION : {
                    enemies[x][y].set_y((x*BLOCK_WIDTH) / 1);
                    enemies[x][y].set_x((y*BLOCK_HEIGHT));
                    enemies[x][y].set_w(BLOCK_WIDTH);
                    enemies[x][y].set_h(BLOCK_HEIGHT);
                } break;
                case world_map::SOIL_COLLISION : {
                    soiltile[x][y].set_y((x*BLOCK_WIDTH) / 1);
                    soiltile[x][y].set_x((y*BLOCK_HEIGHT));
                    soiltile[x][y].set_w(BLOCK_WIDTH);
                    soiltile[x][y].set_h(BLOCK_HEIGHT);
                } break;
                case world_map::SPIKE_COLLISION : {
                    spikes[x][y].set_y((x*BLOCK_WIDTH) / 1);
                    spikes[x][y].set_x((y*BLOCK_HEIGHT));
                    spikes[x][y].set_w(BLOCK_WIDTH);
                    spikes[x][y].set_h(BLOCK_HEIGHT);                   
                } break;
            }
        }
    }
}




void GameState::doRender(SDL_Renderer *renderer)
{
    // set the drawing color to blue
    SDL_SetRenderDrawColor(renderer, 50, 60, 57, 255);

    // clear the screen (to blue)
    SDL_RenderClear(renderer);

    // set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 50, 60, 57, 255);

    // Background
    SDL_Rect bgRect = { (int)((this->scrollX / 17) + this->backdrop.get_x()),(int)((this->scrollY / 17) + this->backdrop.get_y()), this->backdrop.get_w(), this->backdrop.get_h() };
    SDL_RenderCopy(this->get_renderer(), this->get_backdrop_texture(), NULL, &bgRect);


    int x, y;
    for (x = 0; x < MAP_ROWS; ++x)
    {
        for (y = 0; y < MAP_COLUMNS; ++y)
        {
            switch (tilemap[x][y])
            {
                // Block
                case world_map::BLOCK_COLLISION : {
                    SDL_Rect blockRect = { (int)(this->get_scrollX() + tile[x][y].get_x()), (int)(this->get_scrollY() + tile[x][y].get_y()), tile[x][y].get_w(), tile[x][y].get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_block(), NULL , &blockRect);
                } break;
                // Taco
                case world_map::TACO_COLLISION : {
                    SDL_Rect tacoRect = { (int)(this->get_scrollX() + tile[x][y].get_x()), (int)(this->get_scrollY() + tile[x][y].get_y()), tile[x][y].get_w(), tile[x][y].get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_taco(), NULL , &tacoRect);
                } break;
                case world_map::EMEMY_COLLISION : {
                    SDL_Rect enemyRect = { (int)(this->get_scrollX() + enemies[x][y].get_x()), (int)(this->get_scrollY() + enemies[x][y].get_y()), enemies[x][y].get_w(), enemies[x][y].get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_enemy(), NULL , &enemyRect);              
                } break;
                case world_map::SOIL_COLLISION : {
                    SDL_Rect soilRect = { (int)(this->get_scrollX() + soiltile[x][y].get_x()), (int)(this->get_scrollY() + soiltile[x][y].get_y()), soiltile[x][y].get_w(), soiltile[x][y].get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_soil(), NULL , &soilRect);              
                } break;
                case world_map::SPIKE_COLLISION : {
                    SDL_Rect spikeRect = { (int)(this->get_scrollX() + spikes[x][y].get_x()), (int)(this->get_scrollY() + spikes[x][y].get_y()), spikes[x][y].get_w(), spikes[x][y].get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_spike(), NULL , &spikeRect);              
                } break;     
            }
        }
    }

    // draw a rectangle at plyr's position
    SDL_Rect rect = {  (int)(this->get_scrollX() + this->plyr.get_x()), (int)(this->get_scrollY() + this->plyr.get_y()), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopyEx(renderer, this->plyrFrames[this->plyr.get_animFrame()], NULL, &rect, 0, NULL, (SDL_RendererFlip)(this->plyr.get_facingLeft() == 0));

    // draw text rectangle.
    SDL_Rect textRect = { 0, 0, (int) (this->life_label.get_w() / 4), (int) (this->life_label.get_h() / 3.75) };
    SDL_RenderCopy(this->get_renderer(), this->get_life_label_texture(), NULL, &textRect);

    SDL_Rect tERect = { (int) (WINDOW_WIDTH-(WINDOW_WIDTH / 4.8)), 0, (int) (this->taco_label.get_w() / 4), (int) (this->taco_label.get_h() / 3.75) };
    SDL_RenderCopy(this->get_renderer(), this->get_taco_label_texture(), NULL, &tERect);

    SDL_RenderPresent(renderer);    
}

void GameState::process()
{
    // add time
    this->set_time(this->get_time() + 1);

    enemy_movement();

    // plyr movement
    Player *plyr = &this->plyr;
    plyr->set_x(plyr->get_x() + plyr->get_dx());
    plyr->set_y(plyr->get_y() + plyr->get_dy());

    // enemy movement
    for (int i = 0; i < MAP_ROWS; ++i)
    {
        for (int j = 0; j < MAP_COLUMNS; ++j)
        {
            this->enemies[i][j].set_x(this->enemies[i][j].get_x() + this->enemies[i][j].get_dx());
            this->enemies[i][j].set_y(this->enemies[i][j].get_y() + this->enemies[i][j].get_dy());
        }
    }    

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

    // Player Gravity    
    plyr->apply_gravity();

    // // Enemy Gravity
    // for (int i = 0; i < MAP_ROWS; ++i)
    // {
    //     for (int j = 0; j < MAP_COLUMNS; ++j)
    //     {
    //         //this->enemies[i][j].apply_gravity();
    //     }
    // }

    // Scrolling
    this->set_scrollX(-this->plyr.get_x() + WINDOW_WIDTH/2);
    this->set_scrollY(-this->plyr.get_y() + WINDOW_HEIGHT/2);

    if (this->get_scrollX() > 0)
    {
        this->set_scrollX(0);
    }
    // if (this->get_scrollX() < -38000+320)
    // {
    //     this->set_scrollX(-38000+320);
    // }
}

// Represents a collision within the map
// \param plyr represents the first rect
// \param tile represents a 2D array map
// \param i represents the current row
// \param j represents the current column
// \param P_W represents the first rect's width
// \param P_H represents the first rect's height
template <typename T>
int GameState::collision_in_map(T &plyr, Block tile[][MAP_COLUMNS], int i, int j , int P_W, int P_H)
{
    int touched = 0;
    float pw = P_W, ph = P_H;
    float px = plyr.get_x(), py = plyr.get_y();
    float bx = tile[i][j].get_x(), by = tile[i][j].get_y(), bw = tile[i][j].get_w(), bh = tile[i][j].get_h();
            
    if (px+pw/2 > bx && px+pw/2 < bx+bw)
    {
        // Head Bump
        if (py < by+bh && py>by && plyr.get_dy() < 0)
        {
            // correct y
            plyr.set_y(by+bh);
            py = by+bh;

            // bumped our head, stop any jump velocity
            plyr.set_dy(0);
            plyr.set_onBlock();
            touched = 1;
        }
    }
    if (px+pw > bx && px<bx+bw)
    {
        // Head bump
        if (py+ph > by && py < by && plyr.get_dy() > 0)
        {
            // correct y
            plyr.set_y(by-ph);
            py = by-ph;

            //landed on this ledge, stop any jump velocity
            plyr.set_dy(0);
            plyr.set_onBlock();
            touched = 1;
        }
    }
    if (py+ph > by && py<by+bh)
    {
        // Rubbing against right edge
        if (px < bx+bw && px+pw > bx+bw && plyr.get_dx() < 0)
        {
            // correct x
            plyr.set_x(bx+bw);
            px = bx+bw;

            plyr.set_dx(0);
            touched = 1;
        }
        // Rubbing against left edge
        else if (px+pw > bx && px < bx && plyr.get_dx() > 0)
        {
            // correct x
            plyr.set_x(bx-pw);
            px = bx-pw;

            plyr.set_dx(0);
            touched = 1;
        }
    }
    return touched;
}


// DEBUG: Collision detect works properly
void GameState::collisionDetect()
{
    // Check for collision with any tacos
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLUMNS; j++)
        {
            // If the player and taco collide.
            if ((this->tilemap[i][j] == 1) && collide2d(
                this->plyr.get_x(),
                this->plyr.get_y(),
                this->tile[i][j].get_x(),
                this->tile[i][j].get_y(),
                PLAYER_HEIGHT,
                PLAYER_WIDTH,
                BLOCK_WIDTH,
                BLOCK_HEIGHT
            ))
            {
                this->set_tacos_eaten(this->get_tacos_eaten() + 1);
                // Create a rectangle and set the texture to null.
                SDL_Rect tacoRect = { (int)(this->get_scrollX() + tile[i][j].get_x()), (int)(this->get_scrollY() + tile[i][j].get_y()), tile[i][j].get_w(), tile[i][j].get_h() };
                SDL_RenderCopy(this->get_renderer(), NULL, NULL , &tacoRect);
                // Makes sure the collision will not be repeated.
                tilemap[i][j] = -1;
            }
            // If the player and enemy collide.
            else if ((this->tilemap[i][j] == 2) && collide2d(
                this->plyr.get_x(),
                this->plyr.get_y(),
                this->enemies[i][j].get_x(),
                this->enemies[i][j].get_y(),
                PLAYER_HEIGHT,
                PLAYER_WIDTH,
                ENEMY_WIDTH,
                ENEMY_HEIGHT
            ))
            {
                this->set_life(this->get_life() - 1);
                //std::cout << this->life << std::endl;
            }
            else if ((this->tilemap[i][j] == 3) && collide2d(
                this->plyr.get_x(),
                this->plyr.get_y(),
                this->spikes[i][j].get_x(),
                this->spikes[i][j].get_y(),
                PLAYER_HEIGHT,
                PLAYER_WIDTH,
                SPIKE_WIDTH,
                SPIKE_HEIGHT
            ))
            {
                this->set_life(this->get_life() - 1);
            }
        }
    }

    // Check for collision with any blocks (brick blocks)
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLUMNS; j++)
        {
            if (this->tilemap[i][j] == world_map::BLOCK_COLLISION)
            {
                collision_in_map(this->plyr, this->tile, i, j, PLAYER_WIDTH, PLAYER_HEIGHT);
                // Debug onBlock
            }
        }
    }
}




int GameState::processEvents(SDL_Window *window)
{
    SDL_Event event;
    int done = 0;

    //init_coin_texture();
    init_health_texture();

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
                            Mix_PlayChannel(-1, this->get_jump_music(), 0);
                        }
                    break;       
                }
            }
            break;
            case SDL_QUIT:
                // quit out of the game
                done = 1;
            break;
        }
    }

    if (this->get_life() < 0)
    {
        exit(0);
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

