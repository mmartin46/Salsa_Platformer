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
#include "constants.hpp"
#include "player.hpp"


// Handles each block
// within the map.
typedef struct
{
   int x, y, w, h;
} Block;


class GameState
{
   private:
      int time;
      float scrollX;
      // Renderer
      SDL_Renderer *renderer;
      // Images
      SDL_Texture *block;


   public:

      // Players
      Player plyr;
      // Platforms
      std::vector<Block> blocks = std::vector<Block>(NUM_BLOCKS);
      // Images
      std::vector<SDL_Texture*> plyrFrames = std::vector<SDL_Texture*>(2);

      // Block
      inline SDL_Texture* get_block() { return block; }
      inline void set_block(SDL_Texture* b) { block = b; }

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