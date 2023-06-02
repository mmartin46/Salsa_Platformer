#ifndef ENEMY_H
#define ENEMY_H
#include "player.hpp"
#include "game.hpp"

// Represents an enemy
class Enemy : public Player
{
   private:
   /* data */
      float x, y, w, h;
      float dx, dy;
      int onBlock;
      int animFrame;
      bool slowingDown, facingLeft;
   public:
      inline virtual int get_w() { return w; } const
      inline virtual void set_w(int val) { w = val; }
      inline virtual int get_h() { return h; } const
      inline virtual void set_h(int val) { h = val; }    
};


#endif