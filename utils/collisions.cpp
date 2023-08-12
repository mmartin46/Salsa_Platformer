#include "../game.hpp"



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

