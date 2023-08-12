#ifndef COMP_HPP
#define COMP_HPP
#pragma once

#include "player.hpp"

class CompPlayer : public Player
{
   private:
      float x, y;
      float dx, dy;
      int onBlock;
      int animFrame;
      bool slowingDown, facingLeft;
      // Images
      std::vector<SDL_Texture*> plyrFrames = std::vector<SDL_Texture*>(COMP_PLAYER_FRAMES + 1);
   public:
      CompPlayer();
};



#endif