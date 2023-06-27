// John 3:5
#ifndef GAME_H
#define GAME_H


#include "src\include\SDL2\SDL.h"
#include "src\include\SDL2\SDL_image.h"
#include "src\include\SDL2\SDL_mixer.h"
#include "src\include\SDL2\SDL_ttf.h"
#include "cmath"
#include "utilities.cpp"


/* Keeps track of two player coordinates.*/
typedef struct {
    double x_1;
    double x_2;
    double y_1;
    double y_2;
} Coordinates;

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

class Label : public Entity
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
      SDL_Texture* backdrop_texture;
   public:
      inline SDL_Texture* get_backdrop_texture() { return backdrop_texture; }
      inline void set_backdrop_texture(SDL_Texture* b) { backdrop_texture = b; }
};



class GameState
{
   private:
      int time;
      int scrollX;
      int scrollY;
      // Renderer
      SDL_Renderer *renderer;
      // Images
      SDL_Texture *block;
      SDL_Texture *taco;
      
      SDL_Texture *taco_soil;
      SDL_Texture *enemy;
      SDL_Texture *soil;
      SDL_Texture *spike;
      SDL_Texture *lblTexture;
      SDL_Texture *tcoTexture;
      SDL_Texture *transition_texture;

      
      // Fonts
      TTF_Font *life_font;
      TTF_Font *taco_font;

      // Sounds
      int music_channel;
      Mix_Chunk *bg_music, *jump_sound;

      // Health
      int life;
      int tacos_eaten;

      // Background
      Matrix<int> tilemap;
      Matrix<Block> tile;
      Matrix<Soil> soiltile;
      Matrix<Enemy> enemies;
      Matrix<Spike> spikes;

      Label life_label;
      Label taco_label;

      // Backgrounds
      shared_ptr<Backdrop> backdrop;
      shared_ptr<Backdrop> dim_area;
      shared_ptr<Backdrop> black_screen;

      // Handles a majority of the textures
      vector<void (GameState::*)(SDL_Texture *)> spriteVec;
      vector<pair<string, string> > surface_args;
      vector<void (GameState::*)(SDL_Texture *)>::pointer spritePtr, spriteEnd;   

      // Level
      int level_choice;
      int maximum_y;
      int maximum_x;
      int random_int;
      int generate;
      system_clock::time_point start_time = high_resolution_clock::now();


      // Players
      shared_ptr<Player> ptr;
      shared_ptr<Player> cptr;

      // All players
      vector<shared_ptr<Player> > players;

      // Computer Player Tracker
      vector<pair<double, double> > not_moving;

      // Images

      vector<SDL_Texture*> plyrFrames = vector<SDL_Texture*>(4);
      vector<SDL_Texture*> enemyFrames = vector<SDL_Texture*>(2);
   public:
      // Backdrop
      inline shared_ptr<Backdrop> get_backdrop() { return backdrop; }
      inline shared_ptr<Backdrop> get_dim_area() { return dim_area; }
      inline shared_ptr<Backdrop> get_black_background() { return black_screen; }

      // Players
      shared_ptr<Player> get_player() { return ptr; };
      shared_ptr<Player> get_comp_player() { return cptr; }
      vector<shared_ptr<Player> >* get_players() { return &players; };

      void init_landing_rect(Player &);
      void init_running_rect(Player &);


      GameState();
      void init(int);
      virtual ~GameState();
      // Level Generation
      inline void set_level_choice(int val) { level_choice = val; }
      inline int get_level_choice() { return level_choice; }
      inline void set_maximum_y(int my) { maximum_y = my; }
      inline int get_maximum_y() { return maximum_y; }
      inline void set_maximum_x(int mx) { maximum_x = mx; }

      inline int get_generation() { return generate; }
      inline void set_generation(int n) { generate = n; }

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
      inline SDL_Texture* get_taco_soil() { return taco_soil; }
      inline void set_taco_soil(SDL_Texture *s) { taco_soil = s; }





      inline void set_taco_texture(SDL_Texture *t) { tcoTexture = t; }
      inline SDL_Texture* get_taco_texture(SDL_Texture *t) { return tcoTexture; }
      void create_block_textures(SDL_Surface *);


      inline void set_life_label_texture(SDL_Texture *t) { lblTexture = t; }
      inline SDL_Texture* get_life_label_texture() { return lblTexture; } 
      inline void set_taco_label_texture(SDL_Texture *t) { lblTexture = t; }
      inline SDL_Texture* get_taco_label_texture() { return lblTexture; } 
 

      // Level Transition
      void level_transition();
      inline SDL_Texture* get_transition_texture() { return transition_texture; }
      inline void set_transition_texture(SDL_Texture *t) { transition_texture; }

      // Life
      inline int get_life(){ return life; }
      inline void set_life(int l){ life = l; }

      // Tacos Eaten
      inline int get_tacos_eaten(){ return tacos_eaten; }
      inline void set_tacos_eaten(int t) { tacos_eaten = t; }

      // Renderer
      inline SDL_Renderer* get_renderer() { return renderer; } const
      inline void set_renderer(SDL_Renderer *r) { renderer = r; }

      // Time
      inline int get_time() { return time; } const
      inline void set_time(int t) { time = t; }

      inline system_clock::time_point get_game_time() { return start_time; }
      inline void set_game_time(system_clock::time_point t) { start_time = t; }
      
      // Font
      inline TTF_Font* get_life_font() { return life_font; } const
      inline void set_life_font(TTF_Font *f) { life_font = f; }
      inline TTF_Font* get_taco_font() { return taco_font; } const
      inline void set_taco_font(TTF_Font *f) { taco_font = f; }

      // Sounds
      inline Mix_Chunk* get_bg_music() { return bg_music; }
      inline void set_bg_music(Mix_Chunk* bm) { bg_music = bm; }
      inline Mix_Chunk* get_jump_music() { return jump_sound; }
      inline void set_jump_music(Mix_Chunk *j) { jump_sound = j; }


      // Scrolling
      inline float get_scrollX() { return scrollX; } const
      inline void set_scrollX(float x) { scrollX = x; }
      inline float get_scrollY() { return scrollY; } const
      inline void set_scrollY(float y) { scrollY = y; }


      // Functions
      void init_blocks(int);
      void animate();
      void collision_handler();
      int event_handler(SDL_Window*);
      void loadImages();
      void loadGame();
      void render(SDL_Renderer*);
      void enemy_movement();
      void init_health_texture();
      inline int get_random_int() { return random_int; }
      inline void set_random_int(int rn) { random_int = rn; }

      void computer_player_movement();

      void land_animation(Player *plyr);
      void run_animation(Player *plyr);

      template <typename T>
      int collision_in_map(T &plyr, Matrix<Block> &, int i, int j, int, int);

      friend class Enemy;  
};


#endif