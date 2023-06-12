#include "player.hpp"

// Defaults the character's position
// x = 210
// y = 240 
Player::Player() 
{
   this->set_x(210-PLAYER_WIDTH);
   this->set_y(0-PLAYER_HEIGHT);
   this->set_dx(0);
   this->set_dy(0);

   this->reset_onBlock();
   this->set_animFrame(0);
   this->set_facingLeft(true);
   this->set_slowingDown(false);
}

// Sets the character's posiiton
// to the specified x and y values.
Player::Player(int x_pos, int y_pos) 
{
   this->set_x(x_pos-PLAYER_WIDTH);
   this->set_y(y_pos-PLAYER_HEIGHT);
   this->set_dx(0);
   this->set_dy(0);

   this->reset_onBlock();
   this->set_animFrame(0);
   this->set_facingLeft(true);
   this->set_slowingDown(false);
}

void Player::set_player_frame(int n, SDL_Texture* t) 
{ 
   if (n >= this->plyrFrames.size() || n < 0) {
      std::cout << "player_frame: argument is out of bounds" << std::endl;
      exit(1);
   }
   this->plyrFrames.at(n) = t;
}