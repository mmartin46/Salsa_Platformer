#include "game.hpp"

/*

loader.cpp

The file consists of the uploaded textures
of the entities.

*/



// Creates block textures for the blocks
// within the surface_args vector.
void GameState::create_block_textures(SDL_Surface *surface)
{
    // Checking if the vector sizes are equal.
    if (spriteVec.size() != surface_args.size())
    {
        using std::cout;

        cout << "spriteVec size = " << spriteVec.size() << "\n";
        cout << "surface_args size = " << surface_args.size() << "\n";
        cout << "ERROR: Vector sizes not equal!";
        exit(1);
    }

    spriteEnd = spriteVec.data() + spriteVec.size();
    vector<pair<string, string> >::pointer surfPtr = surface_args.data();

    // Sets the surfaces.
    for (spritePtr = spriteVec.data(); spritePtr < spriteEnd; ++spritePtr, ++surfPtr)
    {
        surface = get_surface(surfPtr->first.c_str(), surfPtr->second.c_str());
        (*this.**spritePtr)(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);
    }
}



void GameState::init_health_texture()
{
    char str[128] = "";
    sprintf(str, "Health: %u                      Tacos Eaten: %u                Time: %u", (int) this->get_life(), (int) this->get_tacos_eaten(), (int) (this->get_time()));

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

    SDL_Surface* surface;

    // Loading textures
    for (auto &p : players)
    {
        p->load_landing_textures(this->get_renderer());
        p->load_running_textures(this->get_renderer());
    }

    // Players
    if (this->get_level_choice() < 1)
    {
        // Level Choice
        surface = get_surface("img\\front_drop.png", "Cannot find front_drop.png!\n\n");
        this->get_dim_area()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
        SDL_FreeSurface(surface);

        // Player Frames
        modify_player_textures(this->get_player(), this->get_renderer(), "img\\playerwfire", 
                              "Cannot find playerwfire", PLAYER_WITH_FIRE_FRAMES, surface);        
    }
    else
    {
        // Player Frames
        modify_player_textures(this->get_player(), this->get_renderer(), "img\\player",
                              "Cannot find player", PLAYER_FRAMES, surface);
    }

    // Computer Player Frames
    modify_player_textures(this->get_comp_player(), this->get_renderer(), "img\\complayer",
                           "Cannot find complayer", COMP_PLAYER_FRAMES, surface);

    // Clear just in case there are any sprites within our vectors.
    this->spriteVec.clear();
    this->surface_args.clear();


    // Defualt block textures
    this->spriteVec.push_back(set_block);
    this->spriteVec.push_back(set_soil);
    this->spriteVec.push_back(set_taco_soil);
    this->spriteVec.push_back(set_taco);
    this->spriteVec.push_back(set_enemy);
    this->spriteVec.push_back(set_spike);


    // Default file arguments for get_surface()
    surface_args.push_back({"img\\block.png", "Cannot find block.png!\n\n"});
    surface_args.push_back({"img\\soil.png", "Cannot find soil.png!\n\n"});
    surface_args.push_back({"img\\taco_soil.png", "Cannot find taco_soil.png!\n\n"});
    surface_args.push_back({"img\\taco.png", "Cannot find block.png!\n\n"});
    surface_args.push_back({"img\\enemy.png", "Cannot find enemy.png!\n\n"});
    surface_args.push_back({"img\\spike.png", "Cannot find spike.png!\n\n"});

    vector<string> dont_use  = {"taco", "enemy", "spike"};
    // Loading background texture.


    switch (this->get_level_choice())
    {
        case 0:
            create_block_textures(surface);
            surface = get_surface("img\\build_block.png", "Cannot find build_block.png!\n\n");
            this->get_backdrop()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
            SDL_FreeSurface(surface);  
            break;
        case 1:
            modify_block_textures(this->surface_args, "_1", dont_use);
            create_block_textures(surface);

            surface = get_surface("img\\world_bg_1.png", "Cannot find world_bg_1.png!\n\n");
            this->get_backdrop()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
            SDL_FreeSurface(surface);  
            break;
        case 2:
            modify_block_textures(this->surface_args, "_2", dont_use);
            create_block_textures(surface);

            surface = get_surface("img\\world_bg_2.png", "Cannot find world_bg_2.png!\n\n");
            this->get_backdrop()->set_backdrop_texture(SDL_CreateTextureFromSurface(this->get_renderer(), surface));
            SDL_FreeSurface(surface);  
            break;
        default:
            std::cout << "loadImages(): Level out of range.\n";
            exit(0);
            break;
    }

}

void GameState::loadGame()
{
    const char *font_name = "fonts\\ka1.ttf";
    loadImages();
}


void GameState::init_blocks(int generated)
{
    if (!generated)
    {
        int x, y;

        int choice = this->get_level_choice();
        for (x = 0; x < MAP_ROWS; ++x)
        {
            for (y = 0; y < MAP_COLUMNS; ++y)
            {
                if (choice == 0)
                {
                    tilemap.at(x).at(y) = world_map::map[x][y];
                }
                else if (choice == 1)
                {
                    tilemap.at(x).at(y) = world_map::map_2[x][y];
                }
                else if (choice == 2)
                {
                    tilemap.at(x).at(y) = world_map::map_3[x][y];
                }
            }
        }

        Entity dim;

        // Set the backdrop dimensions
        switch (choice)
        {
            case 0:
                dim.set_x(-300);
                dim.set_y(-300);
                dim.set_h(5000);
                dim.set_w(5000);

                this->get_dim_area()->set_x(0);
                this->get_dim_area()->set_y(0);
                this->get_dim_area()->set_h(1000);
                this->get_dim_area()->set_w(1000);
                break;
            case 1:
                dim.set_x(-50);
                dim.set_y(-200);
                dim.set_h(598);
                dim.set_w(900);
                break;
            case 2:
                dim.set_x(-30);
                dim.set_y(-90);
                dim.set_h(380);
                dim.set_w(640);
                break;
            default:
                std::cout << "init_blocks(): Level out of range!";
                exit(1);
        }


        this->get_backdrop()->set_x(dim.get_x());
        this->get_backdrop()->set_y(dim.get_y());
        this->get_backdrop()->set_h(dim.get_h());
        this->get_backdrop()->set_w(dim.get_w());

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
}

