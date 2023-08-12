#ifndef ENEMY_H
#define ENEMY_H
#pragma once
#include "player.hpp"

// Represents an enemy
class Enemy : public Player
{
   private:
   /* data */
      float x, y, w, h;
      float dx, dy;
      int onBlock;
      int animFrame;
      int landFrame;
      int enemySpeed;
      bool slowingDown, facingLeft;
   public:
      Enemy();

      inline int get_w() { return w; } const
      inline void set_w(int val) { w = val; }
      inline int get_h() { return h; } const
      inline void set_h(int val) { h = val; }    
      inline void set_enemySpeed(int s) { enemySpeed = s; }
      inline int get_enemySpeed() { return enemySpeed; }

      std::vector<SDL_Texture*> landingFrames = std::vector<SDL_Texture*>(6);
};


// Represents a spike.
class Spike : public Enemy
{
   private:
   /* data */
      float x, y, w, h;
      float dx, dy;
      int onBlock;
      int animFrame;
      bool slowingDown, facingLeft;   
};

#endif