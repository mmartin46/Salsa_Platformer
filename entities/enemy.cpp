#include "enemy.hpp"

// Defaults the enemy's position
Enemy::Enemy() 
{
   set_enemySpeed(1); 
   this->set_dx(0);
   this->set_dy(0);

   this->reset_onBlock();
   this->set_animFrame(0);
   this->set_facingLeft(true);
   this->set_slowingDown(false);
}