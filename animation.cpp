#include "game.hpp"

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
        if (this->get_tacos_eaten() > 20)
        {
            this->set_generation(1);
            std::cout << "TODO: Level Completed!\n";
            ++level_choice;
            std::cout << level_choice << std::endl;

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