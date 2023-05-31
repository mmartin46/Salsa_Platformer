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
      float scrollY;
      // Renderer
      SDL_Renderer *renderer;
      // Images
      SDL_Texture *block;
      // Fonts
      TTF_Font *font;

      int tilemap[MAP_ROWS][MAP_COLUMNS];
      Block tile[MAP_ROWS][MAP_COLUMNS];

   public:
      // Players
      Player plyr;
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
      inline float get_scrollY() { return scrollY; } const
      inline void set_scrollY(float y) { scrollY = y; }


      // Functions
      void init_blocks();
      void process();
      void collisionDetect();
      int processEvents(SDL_Window*);
      void loadImages();
      void loadGame();
      void doRender(SDL_Renderer*);
      
};



#endif