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
      Enemy();
      inline virtual int get_w() { return w; } const
      inline virtual void set_w(int val) { w = val; }
      inline virtual int get_h() { return h; } const
      inline virtual void set_h(int val) { h = val; }    
};

// Defaults the enemy's position
Enemy::Enemy() 
{
   this->set_dx(0);
   this->set_dy(0);

   this->reset_onBlock();
   this->set_animFrame(0);
   this->set_facingLeft(true);
   this->set_slowingDown(false);
}

// Represents a spike.
class Spike : public Enemy
{
   private:
   private:
   /* data */
      float x, y, w, h;
      float dx, dy;
      int onBlock;
      int animFrame;
      bool slowingDown, facingLeft;   
};


#endif