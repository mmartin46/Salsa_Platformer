#include "game.hpp"


/* Constructs the gamestate. */
GameState::GameState()
{
    this->set_life(100);
    this->set_tacos_eaten(0);

    this->set_maximum_y(FALL_DEATH);
    this->ptr = shared_ptr<Player>(new Player);
    this->cptr = shared_ptr<Player>(new CompPlayer);
    this->dim_area = shared_ptr<Backdrop>(new Backdrop);
    this->backdrop = shared_ptr<Backdrop>(new Backdrop);
    this->tilemap = Matrix<int> (MAP_ROWS, vector<int>(MAP_COLUMNS));
    this->tile = Matrix<Block> (MAP_ROWS, vector<Block>(MAP_COLUMNS));
    this->soiltile = Matrix<Soil> (MAP_ROWS, vector<Soil>(MAP_COLUMNS));
    this->enemies = Matrix<Enemy> (MAP_ROWS, vector<Enemy>(MAP_COLUMNS));
    this->spikes = Matrix<Spike> (MAP_ROWS, vector<Spike>(MAP_COLUMNS));
    this->set_time(0);
    this->set_scrollX(0);
    this->set_scrollY(0);
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
    using std::cout;
    // Load fonts
    set_life_font(TTF_OpenFont("img\\ka1.ttf", 48));
    if (!this->get_life_font())
    {
        cout << "Cannot find font file!\n\n";
        SDL_Quit();
        exit(1);
    }
    set_taco_font(TTF_OpenFont("img\\ka1.ttf", 48));
    if (!this->get_taco_font())
    {
        cout << "Cannot find font file!\n\n";
        SDL_Quit();
        exit(1);
    }

    // Load Sounds
    this->set_jump_music(Mix_LoadWAV("sounds\\jump.wav"));
    if (!this->get_jump_music())
    {
        cout << ("Cannot locate sounds\\jump.wav!");
        SDL_Quit();
        exit(1);
    }

    if (this->get_level_choice() < 20)
    {
        SDL_Surface *s = get_surface("img\\front_drop.png", "Cannot find front_drop.png!\n\n");
        this->get_dim_area()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), s));
        SDL_FreeSurface(s);
    }

    // Players
    if (this->get_level_choice() < 20)
    {
        // Player Frames
        SDL_Surface* surface = get_surface("img\\playerwfire1.png", "Cannot find playerwfire1.png!\n\n");
        this->get_player()->set_player_frame(0, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);

        surface = get_surface("img\\playerwfire2.png", "Cannot find playerwfire2.png!\n\n");
        this->get_player()->set_player_frame(1, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);

        surface = get_surface("img\\playerwfire3.png", "Cannot find playerwfire3.png!\n\n");
        this->get_player()->set_player_frame(2, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);

        surface = get_surface("img\\playerwfire4.png", "Cannot find playerwfire4.png!\n\n");
        this->get_player()->set_player_frame(3, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);

        surface = get_surface("img\\playerwfire6.png", "Cannot find playerwfire6.png!\n\n");
        this->get_player()->set_player_frame(4, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);       
    }
    else
    {
        // Player Frames
        SDL_Surface* surface;

        for (int i = 0; i < PLAYER_FRAMES; ++i)
        {
            using std::string;
            using std::to_string;
            using std::endl;

            string req = "img\\player" + to_string(i + 1) + ".png";
            string err = "Cannot find player" + to_string(i) + ".png!\n\n";

            surface = get_surface(req.c_str(), err.c_str());
            this->get_player()->set_player_frame(i, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
            SDL_FreeSurface(surface);  
        }   
    }

    // Computer Player Frames
    SDL_Surface *surface = get_surface("img\\complayer1.png", "Cannot find complayer1.png!\n\n");
    this->get_comp_player()->set_player_frame(0, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
    SDL_FreeSurface(surface);

    surface = get_surface("img\\complayer2.png", "Cannot find complayer2.png!\n\n");
    this->get_comp_player()->set_player_frame(1, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
    SDL_FreeSurface(surface);

    surface = get_surface("img\\complayer3.png", "Cannot find complayer3.png!\n\n");
    this->get_comp_player()->set_player_frame(2, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
    SDL_FreeSurface(surface);

    surface = get_surface("img\\complayer4.png", "Cannot find complayer4.png!\n\n");
    this->get_comp_player()->set_player_frame(3, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
    SDL_FreeSurface(surface);

    surface = get_surface("img\\complayer5.png", "Cannot find complayer5.png!\n\n");
    this->get_comp_player()->set_player_frame(4, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
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



    // Loading background texture.
    if (this->get_level_choice() < 20)
    {

        surface = get_surface("img\\build_block.png", "Cannot find build_block.png!\n\n");
        this->get_backdrop()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);  

        // Loading the block's texture.
        surface = get_surface("img\\block.png", "Cannot find block.png!\n\n");
        this->set_block(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);  

        // Loading soil texture.
        surface = get_surface("img\\soil.png", "Cannot find soil.png!\n\n");
        this->set_soil(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);  

        // Loading taco_soil texture.
        surface = get_surface("img\\taco_soil.png", "Cannot find taco_soil.png!\n\n");
        this->set_taco_soil(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);  
    }
    else if (this->get_level_choice() >= 20 && this->get_level_choice() < 40)
    {
        // TODO: Change to the second level
        surface = get_surface("img\\world_bg_1.png", "Cannot find world_bg_1.png!\n\n");
        this->get_backdrop()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);   

        // Loading the block's texture.
        surface = get_surface("img\\block_1.png", "Cannot find block_1.png!\n\n");
        this->set_block(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface); 

        // Loading soil texture.
        surface = get_surface("img\\soil_1.png", "Cannot find soil_1.png!\n\n");
        this->set_soil(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);    

        // Loading taco_soil texture.
        surface = get_surface("img\\taco_soil_1.png", "Cannot find taco_soil_1.png!\n\n");
        this->set_taco_soil(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);  
    }
    else if (this->get_level_choice() >= 40)
    {
        // TODO: Change to the second level
        surface = get_surface("img\\world_bg_2.png", "Cannot find world_bg_2.png!\n\n");
        this->get_backdrop()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);   

        // Loading the block's texture.
        surface = get_surface("img\\block_2.png", "Cannot find block_2.png!\n\n");
        this->set_block(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface); 

        // Loading soil texture.
        surface = get_surface("img\\soil_2.png", "Cannot find soil_2.png!\n\n");
        this->set_soil(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);    

        // Loading taco_soil texture.
        surface = get_surface("img\\taco_soil_2.png", "Cannot find taco_soil_2.png!\n\n");
        this->set_taco_soil(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
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
                tilemap.at(x).at(y) = world_map::map[x][y];
            }
        }
    }
    else if (choice >= 20 && choice < 40)
    {   
        for (x = 0; x < MAP_ROWS; ++x)
        {
            for (y = 0; y < MAP_COLUMNS; ++y)
            {
                tilemap.at(x).at(y) = world_map::map_2[x][y];
            }
        }
    }
    else if (choice >= 40)
    {
        for (x = 0; x < MAP_ROWS; ++x)
        {
            for (y = 0; y < MAP_COLUMNS; ++y)
            {
                tilemap.at(x).at(y) = world_map::map_3[x][y];
            }
        }        
    }


    if (choice < 20)
    {
        this->get_dim_area()->set_x(0);
        this->get_dim_area()->set_y(0);
        this->get_dim_area()->set_h(1000);
        this->get_dim_area()->set_w(1000);

        this->get_backdrop()->set_x(-300);
        this->get_backdrop()->set_y(-300);
        this->get_backdrop()->set_h(5000);
        this->get_backdrop()->set_w(5000);
    }
    else if (choice >= 20 && choice < 40)
    {
        this->get_backdrop()->set_x(-50);
        this->get_backdrop()->set_y(-200);   
        this->get_backdrop()->set_h(598);
        this->get_backdrop()->set_w(900);        
    }
    else if (choice >= 40)
    {
        this->get_backdrop()->set_x(-30);
        this->get_backdrop()->set_y(-90);   
        this->get_backdrop()->set_h(380);
        this->get_backdrop()->set_w(640); 
    }

    // Intialize the map
    for (x = 0; x < MAP_ROWS; ++x)
    {
        for (y = 0; y < MAP_COLUMNS; ++y)
        {
            // DEBUG
            switch (tilemap.at(x).at(y)) 
            {
                case world_map::BLOCK_COLLISION : {
                    tile.at(x).at(y).set_y((x*BLOCK_WIDTH) / 1);
                    tile.at(x).at(y).set_x((y*BLOCK_HEIGHT));
                    tile.at(x).at(y).set_w(BLOCK_WIDTH);
                    tile.at(x).at(y).set_h(BLOCK_HEIGHT);
                } break;
                case world_map::TACO_COLLISION : {
                    tile.at(x).at(y).set_y((x*BLOCK_WIDTH) / 1);
                    tile.at(x).at(y).set_x((y*BLOCK_HEIGHT));
                    tile.at(x).at(y).set_w(BLOCK_WIDTH);
                    tile.at(x).at(y).set_h(BLOCK_HEIGHT);
                } break;
                case world_map::EMEMY_COLLISION : {
                    enemies.at(x).at(y).set_y((x*BLOCK_WIDTH) / 1);
                    enemies.at(x).at(y).set_x((y*BLOCK_HEIGHT));
                    enemies.at(x).at(y).set_w(BLOCK_WIDTH);
                    enemies.at(x).at(y).set_h(BLOCK_HEIGHT);
                } break;
                case world_map::SOIL_COLLISION : {
                    soiltile.at(x).at(y).set_y((x*BLOCK_WIDTH) / 1);
                    soiltile.at(x).at(y).set_x((y*BLOCK_HEIGHT));
                    soiltile.at(x).at(y).set_w(BLOCK_WIDTH);
                    soiltile.at(x).at(y).set_h(BLOCK_HEIGHT);
                } break;
                case world_map::SPIKE_COLLISION : {
                    spikes.at(x).at(y).set_y((x*BLOCK_WIDTH) / 1);
                    spikes.at(x).at(y).set_x((y*BLOCK_HEIGHT));
                    spikes.at(x).at(y).set_w(BLOCK_WIDTH);
                    spikes.at(x).at(y).set_h(BLOCK_HEIGHT);                   
                } break;
                case world_map::TACO_SOIL_COLLISION : {
                    tile.at(x).at(y).set_y((x*BLOCK_WIDTH) / 1);
                    tile.at(x).at(y).set_x((y*BLOCK_HEIGHT));
                    tile.at(x).at(y).set_w(BLOCK_WIDTH);
                    tile.at(x).at(y).set_h(BLOCK_HEIGHT);                   
                }
            }
        }
    }
}




void GameState::doRender(SDL_Renderer *renderer)
{
    // set the drawing color to blue
    //SDL_SetRenderDrawColor(renderer, 50, 60, 57, 255);

    // clear the screen (to blue)
    SDL_RenderClear(renderer);

    // Level color
    if (this->get_level_choice() < 20)
    {
        // Level 1
        SDL_SetRenderDrawColor(renderer, 50, 60, 57, 255);
    }
    else if (this->get_level_choice() >= 20 && this->get_level_choice() < 40)
    {
        // Level 2
        SDL_SetRenderDrawColor(renderer, 73, 118, 201, 255);   
    }
    else if (this->get_level_choice() >= 40)
    {
        // Level 3
        SDL_SetRenderDrawColor(renderer, 73, 118, 201, 255);           
    }

    // Background

    SDL_Rect bgRect = { (int)((this->scrollX / 20) + this->get_backdrop()->get_x()),(int)((this->scrollY / 30) + this->get_backdrop()->get_y()), this->get_backdrop()->get_w(), this->get_backdrop()->get_h() };
    SDL_RenderCopy(this->get_renderer(), this->get_backdrop()->get_backdrop_texture(), NULL, &bgRect);

    int x, y;
    for (x = 0; x < MAP_ROWS; ++x)
    {
        for (y = 0; y < MAP_COLUMNS; ++y)
        {
            switch (tilemap.at(x).at(y))
            {
                // Block
                case world_map::BLOCK_COLLISION : {
                    SDL_Rect blockRect = { static_cast<int>(this->get_scrollX() + tile.at(x).at(y).get_x()), static_cast<int>(this->get_scrollY() + tile.at(x).at(y).get_y()), tile.at(x).at(y).get_w(), tile.at(x).at(y).get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_block(), NULL , &blockRect);
                } break;
                // Taco
                case world_map::TACO_COLLISION : {
                    SDL_Rect tacoRect = { static_cast<int>(this->get_scrollX() + tile.at(x).at(y).get_x()), static_cast<int>(this->get_scrollY() + tile.at(x).at(y).get_y()), tile.at(x).at(y).get_w(), tile.at(x).at(y).get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_taco(), NULL , &tacoRect);
                } break;
                case world_map::EMEMY_COLLISION : {
                    SDL_Rect enemyRect = { static_cast<int>(this->get_scrollX() + enemies.at(x).at(y).get_x()), static_cast<int>(this->get_scrollY() + enemies.at(x).at(y).get_y()), enemies.at(x).at(y).get_w(), enemies.at(x).at(y).get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_enemy(), NULL , &enemyRect);              
                } break;
                case world_map::SOIL_COLLISION : {
                    SDL_Rect soilRect = { static_cast<int>(this->get_scrollX() + soiltile.at(x).at(y).get_x()), static_cast<int>(this->get_scrollY() + soiltile.at(x).at(y).get_y()), soiltile.at(x).at(y).get_w(), soiltile.at(x).at(y).get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_soil(), NULL , &soilRect);              
                } break;
                case world_map::SPIKE_COLLISION : {
                    SDL_Rect spikeRect = { static_cast<int>(this->get_scrollX() + spikes.at(x).at(y).get_x()), static_cast<int>(this->get_scrollY() + spikes.at(x).at(y).get_y()), spikes.at(x).at(y).get_w(), spikes.at(x).at(y).get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_spike(), NULL , &spikeRect);              
                } break;  
                case world_map::TACO_SOIL_COLLISION : {
                    SDL_Rect tacoSoilRect = { static_cast<int>(this->get_scrollX() + tile.at(x).at(y).get_x()), static_cast<int>(this->get_scrollY() + tile.at(x).at(y).get_y()), tile.at(x).at(y).get_w(), tile.at(x).at(y).get_h() };
                    SDL_RenderCopy(this->get_renderer(), this->get_taco_soil(), NULL , &tacoSoilRect);
                }   
            }
        }
    }




    // Players

    // draw a rectangle at plyr's position
    SDL_Rect rect = {  static_cast<int>(this->get_scrollX() + this->get_player()->get_x()), static_cast<int>(this->get_scrollY() + this->get_player()->get_y()), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopyEx(this->get_renderer(), this->get_player()->get_player_frame(this->get_player()->get_animFrame()), NULL, &rect, 0, NULL, (SDL_RendererFlip)(this->get_player()->get_facingLeft() == 0));

    SDL_Rect crect = {  static_cast<int> (this->get_scrollX() + this->get_comp_player()->get_x()), static_cast<int>(this->get_scrollY() + this->get_comp_player()->get_y()), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopyEx(this->get_renderer(), this->get_comp_player()->get_player_frame(this->get_comp_player()->get_animFrame()), NULL, &crect, 0, NULL, (SDL_RendererFlip)(this->get_comp_player()->get_facingLeft() == 0));

    if (this->get_level_choice() < 20)
    {   
        SDL_Rect dimRect = {  static_cast<int>((this->get_scrollX() + this->get_player()->get_x()) - 490), static_cast<int>((this->get_scrollY() + this->get_player()->get_y()) - 450), this->get_dim_area()->get_w(), this->get_dim_area()->get_h() };
        SDL_RenderCopy(this->get_renderer(), this->get_dim_area()->get_backdrop_texture(), NULL, &dimRect);
    }


    // draw text rectangle.
    SDL_Rect textRect = { 0, 0, static_cast<int> (this->life_label.get_w() / 4), static_cast<int> (this->life_label.get_h() / 3.75) };
    SDL_RenderCopy(this->get_renderer(), this->get_life_label_texture(), NULL, &textRect);

    // draw taco text rectangle
    SDL_Rect tERect = { static_cast<int> (WINDOW_WIDTH-(WINDOW_WIDTH / 4.8)), 0, static_cast<int> (this->taco_label.get_w() / 4), static_cast<int> (this->taco_label.get_h() / 3.75) };
    SDL_RenderCopy(this->get_renderer(), this->get_taco_label_texture(), NULL, &tERect);




    SDL_RenderPresent(this->get_renderer());    
}

void GameState::process()
{
    // add time
    this->set_time(this->get_time() + 1);

    enemy_movement();
    computer_player_movement();

    // plyr movement
    shared_ptr<Player> plyr = this->get_player();
    plyr->set_x(plyr->get_x() + plyr->get_dx());
    plyr->set_y(plyr->get_y() + plyr->get_dy());

    shared_ptr<Player> cplyr = this->get_comp_player();
    cplyr->set_x(cplyr->get_x() + cplyr->get_dx());
    cplyr->set_y(cplyr->get_y() + cplyr->get_dy());

    // enemy movement
    for (int i = 0; i < MAP_ROWS; ++i)
    {
        for (int j = 0; j < MAP_COLUMNS; ++j)
        {
            this->enemies.at(i).at(j).set_x(this->enemies.at(i).at(j).get_x() + this->enemies.at(i).at(j).get_dx());
            this->enemies.at(i).at(j).set_y(this->enemies.at(i).at(j).get_y() + this->enemies.at(i).at(j).get_dy());
        }
    }    

    // Idle Animation
    if (plyr->get_dx() == 0 && plyr->get_dy() == 0)
    {
        if (this->get_time() % 100 < 20)
        {
            if ((this->get_time()) % 20 <= 5)
            {
                plyr->set_animFrame(8);
            }
            else if ((this->get_time() % 20) > 20 && (this->get_time() % 20) <= 40)
            {
                plyr->set_animFrame(9);
            }
            else if ((this->get_time() % 20) > 60 && (this->get_time() % 20) < 80)
            {
                plyr->set_animFrame(10);
            }
            else
            {
                plyr->set_animFrame(0);
            }
        }
    }

    if (plyr->get_dx() != 0 && plyr->get_onBlock() && (plyr->get_slowingDown() == false) )
    {
        if (this->get_time() % 20 < 20)
        {
            if ((this->get_time()) % 20 <= 5)
            {
                plyr->set_animFrame(1);
            }
            else if ((this->get_time() % 20) > 5 && (this->get_time() % 20) <= 10)
            {
                plyr->set_animFrame(2);
            }
            else if ((this->get_time() % 20) > 10 && (this->get_time() % 20) < 20)
            {
                plyr->set_animFrame(3);
            }
            else
            {
                plyr->set_animFrame(0);
            }
        }
    }
    if (cplyr->get_dx() != 0 && cplyr->get_onBlock() && (cplyr->get_slowingDown() == false) )
    {
        if (this->get_time() % 20 < 20)
        {
            if ((this->get_time()) % 20 <= 5)
            {
                cplyr->set_animFrame(1);
            }
            else if ((this->get_time() % 20) > 5 && (this->get_time() % 20) <= 10)
            {
                cplyr->set_animFrame(2);
            }
            else if ((this->get_time() % 20) > 10 && (this->get_time() % 20) < 20)
            {
                cplyr->set_animFrame(3);
            }
            else
            {
                cplyr->set_animFrame(0);
            }
        }
    }
    else
    {
        cplyr->set_animFrame(0);
    }

    // Jumping animations
    if (!plyr->get_onBlock())
    {
        plyr->set_animFrame(4);
    }
    if (!cplyr->get_onBlock())
    {
        cplyr->set_animFrame(4);
    }


    // Player Gravity    
    plyr->apply_gravity();
    cplyr->apply_gravity();

    // Scrolling

    // Make sure only the two pointers ptr and plyr
    // are the only ones that point the player.
    if (plyr.use_count() == 2)
    {
        this->set_scrollX(-plyr->get_x() + WINDOW_WIDTH/2);
        this->set_scrollY(-plyr->get_y() + WINDOW_HEIGHT/2);

        if (this->get_scrollX() > 0)
        {
            this->set_scrollX(0);
        }
    }
    // if (this->get_scrollX() < -38000+320)
    // {
    //     this->set_scrollX(-38000+320);
    // }

    // Player falls off screen
    if (plyr->get_y() >= this->get_maximum_y())
    {
        exit(0);        
    }
    if (cplyr->get_y() >= this->get_maximum_y())
    {
        cplyr->set_x(plyr->get_x() - 20);
        cplyr->set_y(plyr->get_y() - 20);       
    }


    if (this->tacos_eaten == 100)
    {
        this->set_life(this->get_life() + 20);
    }
}

// Represents a collision within the map
// \param plyr represents the first rect
// \param tile represents a 2D array map
// \param i represents the current row
// \param j represents the current column
// \param P_W represents the first rect's width
// \param P_H represents the first rect's height
template <typename T>
int GameState::collision_in_map(T &plyr, Matrix<Block> &tile, int i, int j , int P_W, int P_H)
{
    int touched = 0;
    float pw = P_W, ph = P_H;
    float px = plyr.get_x(), py = plyr.get_y();
    float bx = tile.at(i).at(j).get_x(), by = tile.at(i).at(j).get_y(), bw = tile.at(i).at(j).get_w(), bh = tile.at(i).at(j).get_h();
            
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
            plyr.reset_onBlock();
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
            touched = 2;
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
            touched = 3;
        }
        // Rubbing against left edge
        else if (px+pw > bx && px < bx && plyr.get_dx() > 0)
        {
            // correct x
            plyr.set_x(bx-pw);
            px = bx-pw;

            plyr.set_dx(0);
            touched = 4;
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
            if ((this->tilemap.at(i).at(j) == world_map::TACO_COLLISION) && collide2d(
                this->get_player()->get_x(),
                this->get_player()->get_y(),
                this->tile.at(i).at(j).get_x(),
                this->tile.at(i).at(j).get_y(),
                PLAYER_HEIGHT,
                PLAYER_WIDTH,
                BLOCK_WIDTH,
                BLOCK_HEIGHT
            ))
            {
                this->set_tacos_eaten(this->get_tacos_eaten() + 1);
                // Create a rectangle and set the texture to null.
                SDL_Rect tacoRect = { static_cast<int>(this->get_scrollX() + tile.at(i).at(j).get_x()), static_cast<int>(this->get_scrollY() + tile.at(i).at(j).get_y()), tile.at(i).at(j).get_w(), tile.at(i).at(j).get_h() };
                SDL_RenderCopy(this->get_renderer(), NULL, NULL , &tacoRect);
                // Makes sure the collision will not be repeated.
                tilemap.at(i).at(j) = -1;
            }
            // If the computer player and taco collide.
            else if ((this->tilemap.at(i).at(j) == world_map::TACO_COLLISION) && collide2d(
                this->get_comp_player()->get_x(),
                this->get_comp_player()->get_y(),
                this->tile.at(i).at(j).get_x(),
                this->tile.at(i).at(j).get_y(),
                PLAYER_HEIGHT,
                PLAYER_WIDTH,
                BLOCK_WIDTH,
                BLOCK_HEIGHT
            ))
            {
                this->set_tacos_eaten(this->get_tacos_eaten() + 1);
                // Create a rectangle and set the texture to null.
                SDL_Rect tacoRect = { static_cast<int>(this->get_scrollX() + tile.at(i).at(j).get_x()), static_cast<int>(this->get_scrollY() + tile.at(i).at(j).get_y()), tile.at(i).at(j).get_w(), tile.at(i).at(j).get_h() };
                SDL_RenderCopy(this->get_renderer(), NULL, NULL , &tacoRect);
                // Makes sure the collision will not be repeated.
                tilemap.at(i).at(j) = -1;
            }
            // If the player and enemy collide.
            else if ((this->tilemap.at(i).at(j) == world_map::EMEMY_COLLISION) && collide2d(
                this->get_player()->get_x(),
                this->get_player()->get_y(),
                this->enemies.at(i).at(j).get_x(),
                this->enemies.at(i).at(j).get_y(),
                PLAYER_HEIGHT,
                PLAYER_WIDTH,
                ENEMY_WIDTH,
                ENEMY_HEIGHT
            ))
            {
                this->set_life(this->get_life() - 1);
                //cout << this->life << endl;
            }
            else if ((this->tilemap.at(i).at(j) == world_map::SPIKE_COLLISION) && collide2d(
                this->get_player()->get_x(),
                this->get_player()->get_y(),
                this->spikes.at(i).at(j).get_x(),
                this->spikes.at(i).at(j).get_y(),
                PLAYER_HEIGHT,
                PLAYER_WIDTH,
                SPIKE_WIDTH,
                SPIKE_HEIGHT
            ))
            {
                this->set_life(this->get_life() - 1);
            }
            else if ((collide2d(
                this->enemies.at(i).at(j).get_x(),
                this->enemies.at(i).at(j).get_y(),
                this->tile.at(i).at(j).get_x(),
                this->tile.at(i).at(j).get_y(),
                ENEMY_HEIGHT,
                ENEMY_WIDTH,
                BLOCK_WIDTH,
                BLOCK_HEIGHT
            )))
            {
                // DEBUG: Enemy and block collision
                //cout << "EOSH" << endl; 
                this->enemies.at(i).at(j).set_y(this->enemies.at(i).at(j).get_y() - (this->enemies.at(i).at(j).get_enemySpeed()));
            }
            else if ((this->tilemap.at(i).at(j) == world_map::TACO_SOIL_COLLISION) && collide2d(
                this->get_player()->get_x(),
                this->get_player()->get_y(),
                this->tile.at(i).at(j).get_x(),
                this->tile.at(i).at(j).get_y(),
                PLAYER_HEIGHT,
                PLAYER_WIDTH,
                BLOCK_WIDTH,
                BLOCK_HEIGHT)) 
            {

                this->set_tacos_eaten(this->get_tacos_eaten() + 1);
                // Create a rectangle and set the texture to soil.
                soiltile.at(i).at(j).set_y((i*BLOCK_WIDTH) / 1);
                soiltile.at(i).at(j).set_x((j*BLOCK_HEIGHT));
                soiltile.at(i).at(j).set_w(BLOCK_WIDTH);
                soiltile.at(i).at(j).set_h(BLOCK_HEIGHT);   
                // Makes sure the collision will not be repeated.
                tilemap.at(i).at(j) = static_cast<int>(world_map::SOIL_COLLISION);                
            }
            else if ((this->tilemap.at(i).at(j) == world_map::TACO_SOIL_COLLISION) && collide2d(
                this->get_comp_player()->get_x(),
                this->get_comp_player()->get_y(),
                this->tile.at(i).at(j).get_x(),
                this->tile.at(i).at(j).get_y(),
                PLAYER_HEIGHT,
                PLAYER_WIDTH,
                BLOCK_WIDTH,
                BLOCK_HEIGHT)) 
            {
                this->set_tacos_eaten(this->get_tacos_eaten() + 1);
                // Create a rectangle and set the texture to soil.
                soiltile.at(i).at(j).set_y((i*BLOCK_WIDTH) / 1);
                soiltile.at(i).at(j).set_x((j*BLOCK_HEIGHT));
                soiltile.at(i).at(j).set_w(BLOCK_WIDTH);
                soiltile.at(i).at(j).set_h(BLOCK_HEIGHT);      
                // Makes sure the collision will not be repeated.
                tilemap.at(i).at(j) = static_cast<int>(world_map::SOIL_COLLISION);                
            }

        }
    }


    // Check for collision with any blocks (brick blocks)
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLUMNS; j++)
        {
            if (this->tilemap.at(i).at(j) == world_map::BLOCK_COLLISION)
            {
                collision_in_map(*this->get_player(), this->tile, i, j, PLAYER_WIDTH, PLAYER_HEIGHT);
                collision_in_map(*this->get_comp_player(), this->tile, i, j, PLAYER_WIDTH, PLAYER_HEIGHT);

                // TODO: Debug onBlock
                if (collision_in_map(this->enemies.at(i).at(j), this->tile, i, j, ENEMY_WIDTH, ENEMY_HEIGHT))
                {
                    //cout << "EOSH" << endl;
                    this->enemies.at(i).at(j).set_enemySpeed(this->enemies.at(i).at(j).get_enemySpeed() * -1);
                }
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
                        if (this->get_player()->get_onBlock())
                        {
                            this->get_player()->set_dy(PLAYER_JUMP_HEIGHT);
                            this->get_player()->reset_onBlock();
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
        this->get_player()->apply_jump();
    }

    // Walking
    if (state[SDL_SCANCODE_LEFT])
    {
       this->get_player()->apply_left_movement(4);
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
       this->get_player()->apply_right_movement(4);
    }
    else
    {
        this->get_player()->apply_down_movement();
    }
    return done;
}

void GameState::enemy_movement()
{
   for (int i = 0; i < MAP_ROWS; ++i)
   {
      for (int j = 0; j < MAP_COLUMNS; ++j)
      {
        if ((this->get_time() % 300) < 150)
        {
            this->enemies.at(i).at(j).set_y(this->enemies.at(i).at(j).get_y() - (this->enemies.at(i).at(j).get_enemySpeed()));
        }
        else if (this->collision_in_map(this->enemies.at(i).at(j), this->tile, i, j, PLAYER_WIDTH, PLAYER_HEIGHT))
        {
            this->enemies.at(i).at(j).set_y(this->enemies.at(i).at(j).get_y() - (this->enemies.at(i).at(j).get_enemySpeed()) * -1);
        }
        else
        {
            this->enemies.at(i).at(j).set_y(this->enemies.at(i).at(j).get_y() - (this->enemies.at(i).at(j).get_enemySpeed()) * -1);
        }
      }
   }
}


void GameState::computer_player_movement()
{
    Coordinates pc;
    pc.x_1 = this->get_player()->get_x();
    pc.x_2 = this->get_comp_player()->get_x();
    pc.y_1 = this->get_player()->get_y();
    pc.y_2 = this->get_comp_player()->get_y();

    double plyr_distance = get_distances(pc.x_1, pc.x_2, 
                                                      pc.y_1, pc.y_2);

    pair<double, double> cp_c;
    cp_c.first = pc.x_1;
    cp_c.second = pc.x_1;

    // Clear out the vector.
    if (this->not_moving.size() > 20)
    {
        this->not_moving.clear();
    }


    // If the computer player is too far from the screen.
    if (plyr_distance > 400)
    {
        this->get_comp_player()->set_x(this->get_player()->get_x());
        this->get_comp_player()->set_y(this->get_player()->get_y());
    }

    // If the player and computer player are too far
    // distant.
    if (plyr_distance > 30)
    {
        // Right
        double state_1 = get_distances(pc.x_1 + 10, pc.x_2, pc.y_1, pc.y_2);
        // Down
        double state_2 = get_distances(pc.x_1, pc.x_2, pc.y_1 + 10, pc.y_2);
        // Left
        double state_3 = get_distances(pc.x_1 - 10, pc.x_2, pc.y_1, pc.y_2);
        // Up
        double state_4 = get_distances(pc.x_1, pc.x_2, pc.y_1 - 10, pc.y_2);

        // Up-Right
        double state_5 = get_distances(pc.x_1 + 10, pc.x_2, pc.y_1 + 10, pc.y_2);
        // Up-Left
        double state_6 = get_distances(pc.x_1 - 10, pc.x_2, pc.y_1 + 10, pc.y_2);
        // Down-Right
        double state_7 = get_distances(pc.x_1 + 10, pc.x_2, pc.y_1 - 10, pc.y_2);
        // Down-Left
        double state_8 = get_distances(pc.x_1 - 10, pc.x_2, pc.y_1 - 10, pc.y_2);


        vector<double> states = { state_1, state_2, state_3, state_4, state_5, state_6,
                                 state_7, state_8 };

        using std::cout;
        using std::endl;

        // for (const auto &s : states) 
        // {
        //     cout << s << " ";
        // }
        // cout << endl;

        double min_distance = minimum(states); 


        // Left
        if (min_distance == state_1)
        {
            //cout << "LEFT\n";
            this->get_comp_player()->apply_left_movement(3);
        }
        // Up
        else if (min_distance == state_2)
        {
            //cout << "UP\n";
            this->get_comp_player()->apply_up_movement(4);
        }
        // Right
        else if (min_distance == state_3)
        {
            //cout << "RIGHT\n";
            this->get_comp_player()->apply_right_movement(3);
        }
        // Down
        else if (min_distance == state_4)
        {
            //cout << "DOWN\n";
            this->get_comp_player()->apply_down_movement();
        }
        // Up-Left
        else if (min_distance == state_5)
        {   
            //cout << "UP-LEFT\n";
            this->get_comp_player()->apply_up_movement(COMP_PLAYER_HEIGHT - (state_5 / 100));
            this->get_comp_player()->apply_left_movement(3);
        }
        // Up-Right
        else if (min_distance == state_6)
        {   
            //cout << "UP-RIGHT\n";
            this->get_comp_player()->apply_up_movement(COMP_PLAYER_HEIGHT - (state_6 / 100));
            this->get_comp_player()->apply_right_movement(3);
        }
        // Down-Left
        else if (min_distance == state_7)
        {   
            //cout << "DOWN-LEFT\n";
            this->get_comp_player()->apply_down_movement();
            this->get_comp_player()->apply_left_movement(3);
        }
        // Down-Right
        else if (min_distance == state_8)
        {   
            //cout << "DOWN-RIGHT\n";
            this->get_comp_player()->apply_down_movement();
            this->get_comp_player()->apply_right_movement(3);
        }
    }
    else
    {
        // If the player isn't performing any of the moves
        // slow his speed to zero.
        this->get_comp_player()->slow_movement();
        this->not_moving.push_back(cp_c);
    }


}


GameState::~GameState()
{
    // Shutdown game and upload all memory
    SDL_DestroyTexture(this->get_taco());
    SDL_DestroyTexture(this->get_taco_soil());
    SDL_DestroyTexture(this->get_spike());
    SDL_DestroyTexture(this->get_soil());
    SDL_DestroyTexture(this->get_enemy());
    SDL_DestroyTexture(this->get_player()->get_player_frame(0));
    SDL_DestroyTexture(this->get_player()->get_player_frame(1));
    SDL_DestroyTexture(this->get_block());

    if (this->get_life_label_texture() != NULL)
    {
        SDL_DestroyTexture(this->get_life_label_texture());
    }
    if (this->get_taco_label_texture() != NULL)
    {
        SDL_DestroyTexture(this->get_taco_label_texture());
    }
}
