#include "game.hpp"
#include "loader.cpp"
#include "comp_movement.cpp"
#include "collisions.cpp"
#include "animation.cpp"


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
