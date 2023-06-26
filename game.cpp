#include "game.hpp"
#include "loader.cpp"
#include "comp_movement.cpp"
#include "collisions.cpp"
#include "animation.cpp"

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
    this->tilemap = Matrix<int> (MAP_ROWS, vector<int>(MAP_COLUMNS));
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
    this->level_transition();    
    this->init_blocks(this->get_generation());
    this->render(this->get_renderer());
    this->set_life(100);
    this->set_time(0);

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

    // draw taco text rectangle
    SDL_Rect tERect = { static_cast<int> (WINDOW_WIDTH-(WINDOW_WIDTH / 4.8)), 0, static_cast<int> (this->taco_label.get_w() / 4), static_cast<int> (this->taco_label.get_h() / 3.75) };
    SDL_RenderCopy(this->get_renderer(), this->get_taco_label_texture(), NULL, &tERect);




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
    if (this->get_taco_label_texture() != NULL)
    {
        SDL_DestroyTexture(this->get_taco_label_texture());
    }
}
