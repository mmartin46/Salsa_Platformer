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

// Handles each block
// within the map.
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
      int animFrame;
      bool slowingDown, facingLeft;
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



      inline int get_animFrame() { return animFrame; } const
      inline void set_animFrame(int af) { animFrame = af; }
      inline int get_facingLeft() { return facingLeft; } const
      inline void set_facingLeft(int v) { facingLeft = v; }
      inline bool get_slowingDown() { return slowingDown; } const
      inline void set_slowingDown(bool s) { slowingDown = s; }
      
};

class GameState
{
   private:
      int time;
      float scrollX;
      // Renderer
      SDL_Renderer *renderer;
   public:

      // Players
      Player plyr;

      // Platforms
      std::vector<Block> blocks = std::vector<Block>(NUM_BLOCKS);

      // Images
      std::vector<SDL_Texture*> plyrFrames = std::vector<SDL_Texture*>(2);
      SDL_Texture *block;

      // Renderer
      inline SDL_Renderer* get_renderer() { return renderer; } const
      inline void set_renderer(SDL_Renderer *r) { renderer = r; }

      // Time
      inline int get_time() { return time; } const
      inline void set_time(int t) { time = t; }
      
      // Scrolling
      inline float get_scrollX() { return scrollX; } const
      inline void set_scrollX(float x) { scrollX = x; }
};


#endif