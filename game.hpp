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
#include "enemy.hpp"


// An entity is a item, enemy, etc. that
// isn't a player and has a sprite.
class Entity
{
   private:
      int x, y, w, h;
   public:
      inline virtual int get_x() { return x; } const
      inline virtual void set_x(int val) { x = val; }
      inline virtual int get_y() { return y; } const
      inline virtual void set_y(int val) { y = val; }

      inline virtual int get_w() { return w; } const
      inline virtual void set_w(int val) { w = val; }
      inline virtual int get_h() { return h; } const
      inline virtual void set_h(int val) { h = val; }    
};

// Handles each block
// within the map.
class Block : public Entity
{
   private:
      int x, y, w, h;
   public:
      friend class GameState;
};

// Handles each soil block within the map.
class Soil : public Entity
{
   private:
      int x, y, w, h;
   public:
      friend class GameState;
};

// Handles each taco within the map.
class Taco : public Entity
{
   private:
      int x, y, w, h;
   public:
      friend class GameState;
};

class Backdrop : public Entity
{
   private:
      int x, y, w, h;
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
      SDL_Texture *taco;
      SDL_Texture *enemy;
      SDL_Texture *soil;
      SDL_Texture *spike;
      SDL_Texture *backdrop_texture;
      // Fonts
      TTF_Font *font;

      int life = 1000000;


      int tilemap[MAP_ROWS][MAP_COLUMNS];
      Block tile[MAP_ROWS][MAP_COLUMNS];
      Soil soiltile[MAP_ROWS][MAP_COLUMNS];
      Enemy enemies[MAP_ROWS][MAP_COLUMNS];
      Spike spikes[MAP_ROWS][MAP_COLUMNS];
      Backdrop backdrop;
   public:


      // Players
      Player plyr;
      // Images
      std::vector<SDL_Texture*> plyrFrames = std::vector<SDL_Texture*>(2);
      std::vector<SDL_Texture*> enemyFrames = std::vector<SDL_Texture*>(2);


      GameState();

      // Textures
      inline SDL_Texture* get_block() { return block; }
      inline void set_block(SDL_Texture* b) { block = b; }
      inline SDL_Texture* get_taco() { return taco; }
      inline void set_taco(SDL_Texture* t) { taco = t; }
      inline SDL_Texture* get_enemy() { return enemy; }
      inline void set_enemy(SDL_Texture* e) { enemy = e; }
      inline SDL_Texture* get_soil() { return soil; }
      inline void set_soil(SDL_Texture* s) { soil = s; }
      inline SDL_Texture* get_spike() { return spike; }
      inline void set_spike(SDL_Texture* s) { spike = s; }
      inline SDL_Texture* get_backdrop_texture() { return backdrop_texture; }
      inline void set_backdrop_texture(SDL_Texture* b) { backdrop_texture = b; }

      // Life
      inline int get_life(){ return life; }
      inline void set_life(int l){ life = l; }

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
      void enemy_movement();

      template <typename T>
      int collision_in_map(T &plyr, Block tile[][MAP_COLUMNS], int i, int j, int, int);

      friend class Enemy;    
};


void GameState::enemy_movement()
{
   for (int i = 0; i < MAP_ROWS; ++i)
   {
      for (int j = 0; j < MAP_COLUMNS; ++j)
      {
         if ((this->get_time() % 300) < 150)
         {
            this->enemies[i][j].set_y(this->enemies[i][j].get_y() - 1);
         }
         else
         {
            this->enemies[i][j].set_y(this->enemies[i][j].get_y() + 1);           
         }
      
      }
   }
}

#endif