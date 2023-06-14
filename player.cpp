#include "player.hpp"
#include "game.hpp"

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

// Allows the player to move left.
void Player::apply_left_movement()
{
   this->move_left_x();
   if (this->get_dx() < -6)
   {
       this->move_left_dx(); 
   }
   this->set_facingLeft(true);
   this->set_slowingDown(false);
}
// Allows the player to move right.
void Player::apply_right_movement()
{
   this->move_right_x();;
   if (this->get_dx() > 6)
   {
       this->move_right_dx();
   }
   this->set_facingLeft(false);
   this->set_slowingDown(false);
}

// Allows the player to move down.
void Player::apply_down_movement()
{
   // Slows down to 0.
   this->set_animFrame(0);
   this->slow_movement();
   this->set_slowingDown(true);
   if (SDL_fabsf(this->get_dx()) < 0.1f)
   {
       this->apply_static_movement();
   }
}

// Allows the player to move up.
void Player::apply_up_movement()
{
   if (this->get_onBlock())
   {
      this->set_dy(PLAYER_JUMP_HEIGHT);
      this->reset_onBlock();
   }
}