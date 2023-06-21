#include "game.hpp"

// Creates block textures for the blocks
// within the surface_args vector.
void GameState::create_block_textures(SDL_Surface *surface)
{
    for (int i = 0; i < this->spriteVec.size(); ++i)
    {
        surface = get_surface(surface_args.at(i).first.c_str(), surface_args.at(i).second.c_str());
        (*this.*spriteVec[i])(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);
    }
}

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
    using std::string;
    using std::to_string;
    using std::endl;
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
        SDL_Surface* surface;

        for (int i = 0; i < PLAYER_WITH_FIRE_FRAMES; ++i)
        {
            string req = "img\\playerwfire" + to_string(i + 1) + ".png";
            string err = "Cannot find playerwfire" + to_string(i + 1) + ".png!\n\n";

            surface = get_surface(req.c_str(), err.c_str());
            this->get_player()->set_player_frame(i, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
            SDL_FreeSurface(surface);  
        }         
    }
    else
    {
        // Player Frames
        SDL_Surface* surface;

        for (int i = 0; i < PLAYER_FRAMES; ++i)
        {
            string req = "img\\player" + to_string(i + 1) + ".png";
            string err = "Cannot find player" + to_string(i) + ".png!\n\n";

            surface = get_surface(req.c_str(), err.c_str());
            this->get_player()->set_player_frame(i, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
            SDL_FreeSurface(surface);  
        }   
    }

    // Computer Player Frames
    SDL_Surface* surface;
    for (int i = 0; i < COMP_PLAYER_FRAMES; ++i)
    {
        string req = "img\\complayer" + to_string(i + 1) + ".png";
        string err = "Cannot find complayer" + to_string(i + 1) + ".png!\n\n";


        surface = get_surface(req.c_str(), err.c_str());
        this->get_comp_player()->set_player_frame(i, SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);
    }

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

    // Defualt block textures
    this->spriteVec.push_back(set_block);
    this->spriteVec.push_back(set_soil);
    this->spriteVec.push_back(set_taco_soil);

    // Default file arguments for get_surface()
    surface_args.push_back({"img\\block.png", "Cannot find block.png!\n\n"});
    surface_args.push_back({"img\\soil.png", "Cannot find soil.png!\n\n"});
    surface_args.push_back({"img\\taco_soil.png", "Cannot find taco_soil.png!\n\n"});

    // Loading background texture.
    if (this->get_level_choice() < 20)
    {
        create_block_textures(surface);
        surface = get_surface("img\\build_block.png", "Cannot find build_block.png!\n\n");
        this->get_backdrop()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);  
    }
    else if (this->get_level_choice() >= 20 && this->get_level_choice() < 40)
    {
        modify_block_textures(this->surface_args, "_1");
        create_block_textures(surface);

        surface = get_surface("img\\world_bg_1.png", "Cannot find world_bg_1.png!\n\n");
        this->get_backdrop()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);   

    }
    else if (this->get_level_choice() >= 40)
    {
        modify_block_textures(this->surface_args, "_2");
        create_block_textures(surface);

        surface = get_surface("img\\world_bg_2.png", "Cannot find world_bg_2.png!\n\n");
        this->get_backdrop()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
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

