#ifndef ENEMY_H
#define ENEMY_H
#include "player.hpp"
#include "game.hpp"

// Represents an enemy
class Enemy : public Player
{
   private:
   /* data */
      float x, y;
      float dx, dy;
      int onBlock;
      int animFrame;
      bool slowingDown, facingLeft;
};


#endif