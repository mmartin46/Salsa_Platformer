// John 3:5
#ifndef GAME_H
#define GAME_H

#include "src\include\SDL2\SDL.h"
#include "src\include\SDL2\SDL_image.h"
#include "src\include\SDL2\SDL_mixer.h"
#include "src\include\SDL2\SDL_ttf.h"

#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include "constants.hpp"
#include "player.hpp"


// Handles each block
// within the map.
class Block
{
   private:
      int x, y, w, h;
   public:
      inline int get_x() { return x; } const
      inline void set_x(int val) { x = val; }
      inline int get_y() { return y; } const
      inline void set_y(int val) { y = val; }

      inline int get_w() { return w; } const
      inline void set_w(int val) { w = val; }
      inline int get_h() { return h; } const
      inline void set_h(int val) { h = val; }
};


class GameState
{
   private:
      int time;
      float scrollX;
      // Renderer
      SDL_Renderer *renderer;
      // Images
      SDL_Texture *block;
      // Fonts
      TTF_Font *font;

      std::vector<std::vector<int> > tilemap;
   public:
      // Players
      Player plyr;
      // Platforms
      std::vector<Block> blocks = std::vector<Block>(NUM_BLOCKS);
      // Images
      std::vector<SDL_Texture*> plyrFrames = std::vector<SDL_Texture*>(2);


      GameState();

      // Block
      inline SDL_Texture* get_block() { return block; }
      inline void set_block(SDL_Texture* b) { block = b; }

      // Renderer
      inline SDL_Renderer* get_renderer() { return renderer; } const
      inline void set_renderer(SDL_Renderer *r) { renderer = r; }

      // Time
      inline int get_time() { return time; } const
      inline void set_time(int t) { time = t; }
      
      // Font
      inline TTF_Font* get_font() { return font; } const
      inline void set_font(TTF_Font *f) { font = f; }

      // Scrolling
      inline float get_scrollX() { return scrollX; } const
      inline void set_scrollX(float x) { scrollX = x; }

      void process();
      void collisionDetect();
      int processEvents(SDL_Window*);
};



GameState::GameState()
{
   this->set_time(0);
   this->set_scrollX(0);
}

void GameState::process()
{
    // add time
    this->set_time(this->get_time() + 1);

    // plyr movement
    Player *plyr = &this->plyr;
    plyr->set_x(plyr->get_x() + plyr->get_dx());
    plyr->set_y(plyr->get_y() + plyr->get_dy());

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

    plyr->apply_gravity();

    this->set_scrollX(-this->plyr.get_x() + 320);
    
    if (this->get_scrollX() > 0)
    {
        this->set_scrollX(0);
    }
    if (this->get_scrollX() < -38000+320)
    {
        this->set_scrollX(-38000+320);
    }
}


void GameState::collisionDetect()
{
    // Check for collision with any blocks (brick blocks)
    for (int i = 0; i < 100; i++)
    {
        float pw = PLAYER_WIDTH, ph = PLAYER_HEIGHT;
        float px = this->plyr.get_x(), py = this->plyr.get_y();
        float bx = this->blocks.at(i).get_x(), by = this->blocks.at(i).get_y(), bw = this->blocks.at(i).get_w(), bh = this->blocks.at(i).get_h();
    
        if (px+pw/2 > bx && px+pw/2 < bx+bw)
        {
            // Head Bump
            if (py < by+bh && py>by && this->plyr.get_dy() < 0)
            {
                // correct y
                this->plyr.set_y(by+bh);
                py = by+bh;

                // bumped our head, stop any jump velocity
                this->plyr.set_dy(0);
                this->plyr.set_onBlock();
            }
        }
        if (px+pw > bx && px<bx+bw)
        {
            // Head bump
            if (py+ph > by && py < by && this->plyr.get_dy() > 0)
            {
                // correct y
                this->plyr.set_y(by-ph);
                py = by-ph;

                //landed on this ledge, stop any jump velocity
                this->plyr.set_dy(0);
                this->plyr.set_onBlock();
            }
        }
        if (py+ph > by && py<by+bh)
        {
            // Rubbing against right edge
            if (px < bx+bw && px+pw > bx+bw && this->plyr.get_dx() < 0)
            {
                // correct x
                this->plyr.set_x(bx+bw);
                px = bx+bw;

                this->plyr.set_dx(0);
            }
            // Rubbing against left edge
            else if (px+pw > bx && px < bx && this->plyr.get_dx() > 0)
            {
                // correct x
                this->plyr.set_x(bx-pw);
                px = bx-pw;

                this->plyr.set_dx(0);
            }
        }
    }
}


int GameState::processEvents(SDL_Window *window)
{
    SDL_Event event;
    int done = 0;

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
                        }
                }
            }
            break;
            case SDL_QUIT:
                // quit out of the game
                done = 1;
            break;
        }
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


#endif