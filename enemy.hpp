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
   public:
      void enemy_movement(GameState *gameState);
};

void Enemy::enemy_movement(GameState *gameState)
{
   this->apply_gravity();

   if ((gameState->time % 1000) < 500)
   {
      this->move_left_x();
      if (this->get_dx() < -6)
      {
         this->move_left_dx();
      }
   }
   else
   {
      this->move_right_x();
      if (this->get_dx() > 6)
      {
         this->move_right_dx();
      }
   }
}

#endif