#include "game.hpp"

// Allows the players to move
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
        if (this->get_time() % 20 < 20)
        {
            if ((this->get_time()) % 20 <= 5)
            {
                if (this->get_level_choice() < 20)
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
                if (this->get_level_choice() < 20)
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
                if (this->get_level_choice() < 20)
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
                if (this->get_level_choice() < 20)
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



    // Movement Animation
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
            else if ((this->get_time() % 20) > 10 && (this->get_time() % 20) <= 15)
            {
                plyr->set_animFrame(3);
            }
            else if ((this->get_time() % 20) > 15 && (this->get_time() % 20) <= 20)
            {
                plyr->set_animFrame(4);
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
            else if ((this->get_time() % 20) > 10 && (this->get_time() % 20) <= 15)
            {
                cplyr->set_animFrame(3);
            }
            else if ((this->get_time() % 20) > 15 && (this->get_time() % 20) <= 20)
            {
                cplyr->set_animFrame(4);
            }
            else
            {
                cplyr->set_animFrame(0);
            }
        }
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