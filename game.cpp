#include "game.hpp"
#include "utilities.hpp"


/* Constructs the gamestate. */
GameState::GameState()
{
    this->set_life(100);
    this->set_tacos_eaten(0);

    this->set_maximum_y(FALL_DEATH);
    this->ptr = shared_ptr<Player>(new Player);
    this->cptr = shared_ptr<Player>(new CompPlayer);

    this->players = { this->ptr, this->cptr };

    this->dim_area = shared_ptr<Backdrop>(new Backdrop);
    this->backdrop = shared_ptr<Backdrop>(new Backdrop);
    this->layer1 = Matrix<int> (MAP_ROWS, vector<int>(MAP_COLUMNS));
    this->tile = Matrix<Block> (MAP_ROWS, vector<Block>(MAP_COLUMNS));
    this->soiltile = Matrix<Soil> (MAP_ROWS, vector<Soil>(MAP_COLUMNS));
    this->enemies = Matrix<Enemy> (MAP_ROWS, vector<Enemy>(MAP_COLUMNS));
    this->spikes = Matrix<Spike> (MAP_ROWS, vector<Spike>(MAP_COLUMNS));


    this->set_time(0);
    this->set_scrollX(0);
    this->set_scrollY(0);
}


void GameState::init(int level_choice)
{
    this->level_transition();        
    this->set_generation(0);
    this->set_level_choice(level_choice);

    this->get_player()->set_x(210-PLAYER_WIDTH);
    this->get_player()->set_y(0-PLAYER_HEIGHT);
    this->get_player()->set_dx(0);
    this->get_player()->set_dy(0);


    this->get_comp_player()->set_x(190-PLAYER_WIDTH);
    this->get_comp_player()->set_y(0-PLAYER_HEIGHT);
    this->get_comp_player()->set_dx(0);
    this->get_comp_player()->set_dy(0);



    this->set_tacos_eaten(0);
    this->loadImages();
    this->init_blocks(this->get_generation());
    this->render(this->get_renderer());
    this->set_life(100);
    this->set_time(0);
    this->set_game_time(high_resolution_clock::now());
}

void GameState::render(SDL_Renderer *renderer)
{

    // clear the screen (to blue)
    SDL_RenderClear(renderer);

    // Level color
    switch (this->get_level_choice())
    {
        case 0:
            SDL_SetRenderDrawColor(renderer, 50, 60, 57, 255);
            break;
        case 1:
            SDL_SetRenderDrawColor(renderer, 50, 60, 57, 255);
            break;
        case 2:
            SDL_SetRenderDrawColor(renderer, 73, 118, 201, 255);
            break;
        default:
            std::cout << "render(): Level out of bounds\n";
            exit(1);
    }




    // Background
    SDL_Rect bgRect = { (int)((this->scrollX / 20) + this->get_backdrop()->get_x()),(int)((this->scrollY / 30) + this->get_backdrop()->get_y()), this->get_backdrop()->get_w(), this->get_backdrop()->get_h() };
    SDL_RenderCopy(this->get_renderer(), this->get_backdrop()->get_backdrop_texture(), NULL, &bgRect);



    // Block Rendering
    int x, y;
    for (x = 0; x < MAP_ROWS; ++x)
    {
        for (y = 0; y < MAP_COLUMNS; ++y)
        {
            switch (layer1.at(x).at(y))
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

    // draw a rectangle at he player's position.
    for (auto &p : *this->get_players())
    {
        SDL_Rect prect = {  static_cast<int> (this->get_scrollX() + p->get_x()), static_cast<int>(this->get_scrollY() + p->get_y()), PLAYER_WIDTH, PLAYER_HEIGHT };
        SDL_RenderCopyEx(this->get_renderer(), p->get_player_frame(p->get_animFrame()), NULL, &prect, 0, NULL, (SDL_RendererFlip)(p->get_facingLeft() == 0));
        // intializing landing animations
        init_landing_rect(*p);
        // initializing running animations
        init_running_rect(*p);
    }


    if (this->get_level_choice() < 1)
    {   
        SDL_Rect dimRect = {  static_cast<int>((this->get_scrollX() + this->get_player()->get_x()) - 490), static_cast<int>((this->get_scrollY() + this->get_player()->get_y()) - 450), this->get_dim_area()->get_w(), this->get_dim_area()->get_h() };
        SDL_RenderCopy(this->get_renderer(), this->get_dim_area()->get_backdrop_texture(), NULL, &dimRect);
    }


    // draw text rectangle.
    SDL_Rect textRect = { 0, 0, static_cast<int> (this->life_label.get_w() / 4), static_cast<int> (this->life_label.get_h() / 3.75) };
    SDL_RenderCopy(this->get_renderer(), this->get_life_label_texture(), NULL, &textRect);




    SDL_RenderPresent(this->get_renderer());    
}


// Initializes the SDL_Rect for the landing
// animation texture.
void GameState::init_landing_rect(Player &plyr)
{
    SDL_Rect landRect = {  static_cast<int>((this->get_scrollX() + plyr.get_x()) - 1), static_cast<int>(this->get_scrollY() + plyr.get_y()), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopyEx(this->get_renderer(), plyr.get_landing_frame(plyr.get_landFrame()), NULL, &landRect, 0, NULL, (SDL_RendererFlip)(plyr.get_facingLeft() == 0));    
}

// Initializes the SDL_Rect for the running
// animation texture.
void GameState::init_running_rect(Player &plyr)
{
    int distance = -13;
    if (plyr.get_facingLeft() == 0)
    {
        distance *= -1;
    }
    SDL_Rect runRect = {  static_cast<int>((this->get_scrollX() + plyr.get_x()) - distance), static_cast<int>(this->get_scrollY() + plyr.get_y()), PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopyEx(this->get_renderer(), plyr.get_running_frame(plyr.get_runFrame()), NULL, &runRect, 0, NULL, (SDL_RendererFlip)(plyr.get_facingLeft() == 0));      
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
    Mix_FreeChunk(this->get_jump_music());

    if (this->get_life_label_texture() != NULL)
    {
        SDL_DestroyTexture(this->get_life_label_texture());
    }
}

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
    using namespace std::chrono;
    auto current_time = high_resolution_clock::now();
    
    auto time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
    char str[128] = "";
    sprintf(str, "Health: %u                      Tacos Eaten: %u                Time: %u", (int) this->get_life(), (int) this->get_tacos_eaten(), (time));

    SDL_Color white = { 255, 255, 255, 255 };

    SDL_Surface *tmp = TTF_RenderText_Blended(this->get_life_font(), str, white);
    this->life_label.set_w(tmp->w);
    this->life_label.set_h(tmp->h);
    this->set_life_label_texture(SDL_CreateTextureFromSurface(this->get_renderer(), tmp));
    SDL_FreeSurface(tmp);
}

void GameState::level_transition(const char *lvl)
{
    using namespace std::this_thread;
    using namespace std::chrono_literals;

    char str[128] = "";
    if ((this->get_level_choice() + 1) < MAX_LEVELS)
    {
        sprintf(str, lvl, (int) (this->get_level_choice() + 1));
    }
    else
    {
        sprintf(str, "You Win!");
    }

    // Set the screen to black and clear the renderer.
    SDL_SetRenderDrawColor(this->get_renderer(), 0, 0, 0, 255);
    SDL_RenderClear(this->get_renderer());

    SDL_Color white = { 255, 255, 255, 255 };

    SDL_Surface *tmp = TTF_RenderText_Blended(this->get_stage_font(), str, white);
    this->stage_label.set_w(tmp->w);
    this->stage_label.set_h(tmp->h);
    this->set_stage_label_texture(SDL_CreateTextureFromSurface(this->get_renderer(), tmp));
    SDL_FreeSurface(tmp);

    SDL_Rect stageRect = { (WINDOW_WIDTH / 2) - 140, (WINDOW_HEIGHT / 2) - 30, this->stage_label.get_w(), this->stage_label.get_h() };
    SDL_RenderCopy(this->get_renderer(), this->get_stage_label_texture(), NULL, &stageRect );

    SDL_RenderPresent(this->get_renderer());
    sleep_for(1s);
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
        cout << "life_font: Cannot find font file!\n\n";
        SDL_Quit();
        exit(1);
    }
    set_taco_font(TTF_OpenFont("img\\ka1.ttf", 48));
    if (!this->get_taco_font())
    {
        cout << "taco_font: Cannot find font file!\n\n";
        SDL_Quit();
        exit(1);
    }

    set_stage_font(TTF_OpenFont("img\\ka1.ttf", 48));
    if (!this->get_stage_font())
    {
        cout << "stage_font: Cannot find font file!\n\n";
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
    this->spriteVec.push_back(set_transition_texture);


    // Default file arguments for get_surface()
    surface_args.push_back({"img\\block.png", "Cannot find block.png!\n\n"});
    surface_args.push_back({"img\\soil.png", "Cannot find soil.png!\n\n"});
    surface_args.push_back({"img\\taco_soil.png", "Cannot find taco_soil.png!\n\n"});
    surface_args.push_back({"img\\taco.png", "Cannot find block.png!\n\n"});
    surface_args.push_back({"img\\enemy.png", "Cannot find enemy.png!\n\n"});
    surface_args.push_back({"img\\spike.png", "Cannot find spike.png!\n\n"});
    surface_args.push_back({"img\\transition_backdrop.png", "Cannot find transition_backdrop.png!\n\n"});

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
                    layer1.at(x).at(y) = world_map::map[x][y];
                }
                else if (choice == 1)
                {
                    layer1.at(x).at(y) = world_map::map_2[x][y];
                }
                else if (choice == 2)
                {
                    layer1.at(x).at(y) = world_map::map_3[x][y];
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
                switch (layer1.at(x).at(y)) 
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
    else
    {
        SDL_RenderClear(this->get_renderer());
    }
}


/*

comp_movement.cpp

The file consists of functions/methods dedicating how 
the computer player is supposed to move.

*/


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

/*

collision.cpp

The file consists of functions dedicating
how the entities react when a collision occurs.

*/



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

            if (plyr.get_onBlock() && (plyr.get_dx() != 0))
            {
                this->run_animation(&plyr);
            }

            this->land_animation(&plyr);
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


// Handles the collisions within the game.
void GameState::collision_handler()
{
    // Check for collision with any tacos
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLUMNS; j++)
        {
            // If the player and taco collide.
            if ((this->layer1.at(i).at(j) == world_map::TACO_COLLISION) && collide2d(
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
                layer1.at(i).at(j) = -1;
            }
            // If the computer player and taco collide.
            else if ((this->layer1.at(i).at(j) == world_map::TACO_COLLISION) && collide2d(
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
                layer1.at(i).at(j) = -1;
            }
            // If the player and enemy collide.
            else if ((this->layer1.at(i).at(j) == world_map::EMEMY_COLLISION) && collide2d(
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
            else if ((this->layer1.at(i).at(j) == world_map::SPIKE_COLLISION) && collide2d(
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
            else if ((this->layer1.at(i).at(j) == world_map::TACO_SOIL_COLLISION) && collide2d(
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
                layer1.at(i).at(j) = static_cast<int>(world_map::SOIL_COLLISION);                
            }
            else if ((this->layer1.at(i).at(j) == world_map::TACO_SOIL_COLLISION) && collide2d(
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
                layer1.at(i).at(j) = static_cast<int>(world_map::SOIL_COLLISION);                
            }

        }
    }


    // Check for collision with any blocks (brick blocks)
    for (int i = 0; i < MAP_ROWS; i++)
    {
        for (int j = 0; j < MAP_COLUMNS; j++)
        {
            if (this->layer1.at(i).at(j) == world_map::BLOCK_COLLISION)
            {

                for (auto &p : players)
                {
                    collision_in_map(*p, this->tile, i, j, PLAYER_WIDTH, PLAYER_HEIGHT);
                }
                // // TODO: Debug onBlock
                // if (collision_in_map(this->enemies.at(i).at(j), this->tile, i, j, ENEMY_WIDTH, ENEMY_HEIGHT))
                // {
                //     //cout << "EOSH" << endl;
                //     this->enemies.at(i).at(j).set_enemySpeed(this->enemies.at(i).at(j).get_enemySpeed() * -1);
                // }
            }
        }
    }
}


/*

animation.cpp

The file consists of the controlled animation
of the entities.

*/


// Allows the players to move
void GameState::animate()
{
    if (this->get_generation() == 0)
    {
        // add time
        this->set_time(this->get_time() + 1);

        enemy_movement();
        computer_player_movement();

        // plyr movement
        shared_ptr<Player> plyr = this->get_player();
        shared_ptr<Player> cplyr = this->get_comp_player();


        for (auto &p : *this->get_players())
        {
            p->set_x(p->get_x() + p->get_dx());
            p->set_y(p->get_y() + p->get_dy());

            if (!p->get_onBlock())
            {
                p->set_landed(0);
                p->set_landFrame(0);
            }
 
            // Landing Animation
            if (!p->get_onBlock())
            {
                p->set_landed(0);
                p->set_landFrame(0);
            }

            // Running Animation
            if (!p->get_dx() || (p->get_dy() != 0))
            {
                p->set_runFrame(0);
            }
 
        }


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
            if (this->get_time() % 20 < 20)
            {
                if ((this->get_time()) % 20 <= 5)
                {
                    if (this->get_level_choice() < 1)
                    {
                        plyr->set_animFrame(6);
                    }
                    else
                    {
                        plyr->set_animFrame(7);
                    }
                }
                else if ((this->get_time() % 20) > 5 && (this->get_time() % 20) <= 10)
                {
                    if (this->get_level_choice() < 1)
                    {
                        plyr->set_animFrame(7);
                    }
                    else
                    {
                        plyr->set_animFrame(8);
                    }
                }
                else if ((this->get_time() % 20) > 10 && (this->get_time() % 20) < 20)
                {
                    if (this->get_level_choice() < 1)
                    {
                        plyr->set_animFrame(8);
                    }
                    else
                    {
                        plyr->set_animFrame(9);
                    }
                }
                else
                {
                    if (this->get_level_choice() < 1)
                    {
                        plyr->set_animFrame(6);
                    }
                    else
                    {
                        plyr->set_animFrame(7);
                    }
                }
            }
        }

        if (cplyr->get_dx() == 0 && cplyr->get_dy() == 0)
        {
            if (this->get_time() % 20 < 20)
            {
                if ((this->get_time()) % 20 <= 5)
                {
                    cplyr->set_animFrame(5);
                }
                else if ((this->get_time() % 20) > 5 && (this->get_time() % 20) <= 7.5)
                {
                    cplyr->set_animFrame(6);
                }
                else if ((this->get_time() % 20) > 7.5 && (this->get_time() % 20) <= 10)
                {
                    cplyr->set_animFrame(7);
                }
                else if ((this->get_time() % 20) > 10 && (this->get_time() % 20) <= 15)
                {
                    cplyr->set_animFrame(8);
                }
                else
                {
                    cplyr->set_animFrame(6);
                }
            }
        }


        for (auto &p : *this->get_players())
        {
            // Movement Animation
            if (p->get_dx() != 0 && p->get_onBlock() && (p->get_slowingDown() == false) )
            {
                if (this->get_time() % 20 < 20)
                {
                    if ((this->get_time()) % 20 <= 5)
                    {
                        p->set_animFrame(1);
                    }
                    else if ((this->get_time() % 20) > 5 && (this->get_time() % 20) <= 10)
                    {
                        p->set_animFrame(2);
                    }
                    else if ((this->get_time() % 20) > 10 && (this->get_time() % 20) <= 15)
                    {
                        p->set_animFrame(3);
                    }
                    else if ((this->get_time() % 20) > 15 && (this->get_time() % 20) <= 20)
                    {
                        p->set_animFrame(4);
                    }
                    else
                    {
                        p->set_animFrame(0);
                    }
                }
            }

            // Jumping animations
            if (!p->get_onBlock())
            {
                p->set_animFrame(4);
            }

            // Player Gravity    
            p->apply_gravity();
        }




        // Scrolling

        // Make sure only the two pointers ptr and plyr
        // are the only ones that point the player.
        if (plyr.use_count() >= 2)
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


        // If the tacos eaten modulus 100 is equal
        // to zero set the life to 100.
        if ((this->get_tacos_eaten() % 100) == 0 &&
            (this->get_tacos_eaten() != 0))
        {
            this->set_life(100);
        }

        // Completing the level
        if (this->get_tacos_eaten() >= 150)
        {
            this->set_generation(1);
            ++level_choice;
            this->level_transition();
            this->init(level_choice);
        }
    }
}


int GameState::event_handler(SDL_Window *window)
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

// Controls the landing animation.
void GameState::land_animation(Player *plyr)
{
    SDL_Surface *surface;

    if ((this->get_time() % 20 < 20) && !plyr->get_landed())
    {
        if ((((this->get_time()) % 20 <= 5)))
        {
            plyr->set_landFrame(0);
        }
        else if ((this->get_time() % 20) > 5 && (this->get_time() % 20) <= 7.5)
        {
            plyr->set_landFrame(2);
        }
        else if ((this->get_time() % 20) > 7.5 && (this->get_time() % 20) <= 10)
        {
            plyr->set_landFrame(3);
        }
        else if ((this->get_time() % 20) > 10 && (this->get_time() % 20) <= 15)
        {
            plyr->set_landFrame(4);
        }
        else
        {
            plyr->set_landFrame(5);
            plyr->set_landed(1);
        }
    }
}

// Controls the running aniamtion.
void GameState::run_animation(Player *plyr)
{
    SDL_Surface *surface;

    if (plyr->get_onBlock() && (plyr->get_dy() == 0))
    {

        if ((this->get_time() % 17 < 17))
        {
            if ((((this->get_time()) % 17) <= 3.5))
            {
                plyr->set_runFrame(0);
            }
            else if (((this->get_time() % 17) > 3.5) && ((this->get_time() % 17) <= 7.5))
            {
                plyr->set_runFrame(1);
            }
            else if (((this->get_time() % 17) > 7.5) && ((this->get_time() % 17) <= 10))
            {
                plyr->set_runFrame(2);
            }
            else if (((this->get_time() % 17) > 10) && ((this->get_time() % 17) <= 13.5))
            {
                plyr->set_runFrame(3);
            }
            else if (((this->get_time() % 17) > 13.5) && ((this->get_time() % 17) < 17))
            {
                plyr->set_runFrame(4);
            }
        }
    }
    else
    {
        plyr->set_runFrame(0);
    }

}