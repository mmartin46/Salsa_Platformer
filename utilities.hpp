#ifndef UTILITIES_H
#define UTILITIES_H

#include <utility>
#include <ctime>
#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <memory>
#include <algorithm>
#include <functional>
#include "constants.hpp"
#include "player.hpp"
#include "comp.hpp"
#include "enemy.hpp"
#include "entity.hpp"
#include <thread>
#include <chrono>
#include "game.hpp"

using std::vector;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;
template <typename T>
using Matrix = vector<vector<T> >;




// Returns the minimum in a vector.
double minimum(const vector<double> &v);

// Are two rectangles colliding.
int collide2d(float x1, float y1, float x2, float y2, float ht1, float wt1, float wt2, float ht2);

// Allows the user to load a texture and returns a surface.
//
// Parameters:
// \param file includes the file path
// \param error_msg includes an error message if the file isn't found.
SDL_Surface* get_surface(const char *file, const char *error_msg);

// Uses the manhattan distance formula
double get_distances(double x_1, double x_2, double y_1, double y_2);

// Modify the block texture textures depending on the level.
// \param surface_args - the file and error arguments for get_surface()
// \param idx - the index you want to add to select the level.
void modify_block_textures(vector<pair<string,string> > &surface_args, string idx);

// Modify the player textures depending on the level and player.
// \param plyr - shared pointer to the player
// \param r - the game renderer
// \param s1 - picture uploaded
// \param s2 - given error message
// \param size - the number of frames
// \param surface - the surface
void modify_player_textures(shared_ptr<Player> plyr, SDL_Renderer *r, const char *s1, const char *s2, int size, SDL_Surface *surface);

#endif