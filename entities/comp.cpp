#include "comp.hpp"


CompPlayer::CompPlayer()
{
   this->set_x(190-PLAYER_WIDTH);
   this->set_y(0-PLAYER_HEIGHT);
   this->set_dx(0);
   this->set_dy(0);

   this->reset_onBlock();
   this->set_animFrame(0);
   this->set_facingLeft(true);
   this->set_slowingDown(false);
}