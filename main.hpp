// John 3:5
#ifndef MAIN_H
#define MAIN_H

#include "src\include\SDL2\SDL.h"
#include "src\include\SDL2\SDL_image.h"
//#include "src\include\SDL2\SDL_mixer.h"
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "src\include\SDL2\SDL.h"
#include "src\include\SDL2\SDL_image.h"
#include <stdio.h>
#include <iostream>
#include <time.h>


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define NUM_BLOCKS 100
#define BLOCK_HEIGHT 20
#define BLOCK_WIDTH 60

#define GRAVITY 0.5f
#define PLAYER_WIDTH 20 // The player width
#define PLAYER_HEIGHT 20 // The player height
#define PLAYER_SPEED 0.5f   // The player walking speed
#define PLAYER_JUMP_HEIGHT -8 // The player jump height


typedef struct
{
   int x, y, w, h;
} Block;

class Player
{
   private:
      /* data */
      float x, y;
      float dx, dy;
      int onBlock;
      short life;
      char *name;

   public:
      
      inline int get_x() { return x; } const
      inline void set_x(int val) { x = val; }
      inline int get_y() { return y; } const
      inline void set_y(int val) { y = val; }

      inline int get_dx() { return dx; } const
      inline void set_dx(int val) { dx = val; }
      inline int get_dy() { return dy; } const
      inline void set_dy(int val) { dy = val; }


      // Player Phsyics 

      inline void apply_gravity() { dy += GRAVITY; } // Accumulates the gravity constant to the player.

      inline void apply_jump() { dy -= 0.3f; }
      
      inline void move_left_x() { dx += -PLAYER_SPEED; }
      inline void move_right_x() { dx += PLAYER_SPEED; }

      inline void move_left_dx() { dx = -6; }
      inline void move_right_dx() { dx = 6; }
      inline void slow_movement() { dx *= 0.8; };
      inline void apply_static_movement() { dx = 0; }
      inline void set_onBlock() { onBlock = 1; }
      inline void reset_onBlock() { onBlock = 0; }
      inline int get_onBlock() { return onBlock; }



      int animFrame, facingLeft, slowingDown;
};

class GameState
{
   private:
      int time;
   public:
      float scrollX;

      // Players
      Player plyr;

      // Platforms
      std::vector<Block> blocks = std::vector<Block>(NUM_BLOCKS);

      // Images
      SDL_Texture *plyrFrames[2];
      SDL_Texture *block;
      // Renderer
      SDL_Renderer *renderer;

      inline int get_time() { return time; } const
      inline void set_time(int t) { time = t; }

      friend void process(GameState *gameState);
};


#endif