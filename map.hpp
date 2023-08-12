#ifndef MAP_H
#define MAP_H
#pragma once
#include "constants.hpp"

namespace world_map {


   extern int map[MAP_ROWS][MAP_COLUMNS];

   extern int map_2[MAP_ROWS][MAP_COLUMNS];

   extern int map_3[MAP_ROWS][MAP_COLUMNS];
   
   // Constants that represent if
   // there has been a collision
   // between specific blocks.
   enum collisions {
      NO_COLLISION = 0,
      BLOCK_COLLISION = 1,
      TACO_COLLISION = 2,
      EMEMY_COLLISION = 3,
      SPIKE_COLLISION = 4,
      SOIL_COLLISION = 5,
      TACO_SOIL_COLLISION = 6
   };


}



#endif