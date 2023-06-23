#ifndef PLAYER_H
#define PLAYER_H
#include "utilities.hpp"
class Player
{
   private:
      /* data */
      float x, y;
      float dx, dy;
      int onBlock;
      bool landed;
      int animFrame;
      int landFrame = 0;
      bool slowingDown, facingLeft;
      // Images
      std::vector<SDL_Texture*> plyrFrames = std::vector<SDL_Texture*>(PLAYER_FRAMES + 1);
      std::vector<SDL_Texture*> landingFrames = std::vector<SDL_Texture*>(6);

   public:
      Player();
      Player(int, int);
      
      // Player Coordinates
      inline virtual int get_x() { return x; } const
      inline virtual void set_x(int val) { x = val; }
      inline virtual int get_y() { return y; } const
      inline virtual void set_y(int val) { y = val; }

      inline virtual int get_dx() { return dx; } const
      inline virtual void set_dx(int val) { dx = val; }
      inline virtual int get_dy() { return dy; } const
      inline virtual void set_dy(int val) { dy = val; }

      // Player Textures
      void set_player_frame(int n, SDL_Texture* t);
      inline virtual SDL_Texture* get_player_frame(int n) { return plyrFrames.at(n); }


      // Player Physics 

      inline virtual void apply_gravity() { dy += GRAVITY; } // Accumulates the gravity constant to the player.
            
      inline virtual void move_left_x() { dx += -PLAYER_SPEED; }
      inline virtual void move_right_x() { dx += PLAYER_SPEED; }

      inline virtual void move_left_dx() { dx = -PLAYER_SPEED_DX; }
      inline virtual void move_right_dx() { dx = PLAYER_SPEED_DX; }
      inline virtual void slow_movement() { dx *= 0.8; };
      inline virtual void apply_static_movement() { dx = 0; }
      inline virtual void set_onBlock() { onBlock = 1; }
      inline virtual void reset_onBlock() { onBlock = 0; }
      inline virtual int get_onBlock() { return onBlock; }

      // General Movement
      virtual void apply_left_movement(int);
      virtual void apply_right_movement(int);
      inline virtual void apply_jump() { dy -= 0.3f; }

      virtual void apply_up_movement(int);
      virtual void apply_down_movement();


      void set_landing_frame(int n, SDL_Texture *t);
      inline SDL_Texture* get_landing_frame(int n) { return landingFrames.at(n); } ;
      void load_landing_textures(SDL_Renderer *);

      inline virtual bool get_landed() { return landed; } const
      inline virtual void set_landed(bool l) { landed = l; }


      // Player Animation 
      inline virtual int get_animFrame() { return animFrame; } const
      inline virtual void set_animFrame(int af) { animFrame = af; }

      inline virtual int get_landFrame() { return landFrame; } const
      inline virtual void set_landFrame(int lf) { landFrame = lf; }

      inline virtual int get_facingLeft() { return facingLeft; } const
      inline virtual void set_facingLeft(int v) { facingLeft = v; }
      inline virtual bool get_slowingDown() { return slowingDown; } const
      inline virtual void set_slowingDown(bool s) { slowingDown = s; }

      
      //friend class GameState;
};

void Player::load_landing_textures(SDL_Renderer *renderer)
{
    SDL_Surface *surface;
    std::string req, err;
    using std::to_string;
    for (int i = 0; i < 6; ++i)
    {
        req = "img\\landing" + to_string(i + 1) + ".png";
    
        surface = IMG_Load(req.c_str());
        if (surface == NULL)
        {
            printf(err.c_str());
            SDL_Quit();
            exit(1);
        }
        this->set_landing_frame(i, SDL_CreateTextureFromSurface(renderer, surface));
        SDL_FreeSurface(surface);
    }
}

void Player::set_landing_frame(int n, SDL_Texture *t)
{
    using std::cout;
    
    if (n >= this->landingFrames.size() || n < 0) 
    {
        cout << "landingFrames size: " << this->landingFrames.size() << "\n"; 
        cout << "landingFrames: argument is out of bounds\n";
        exit(1);
    }
    this->landingFrames.at(n) = t;
}


#endif