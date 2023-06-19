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
#include "constants.hpp"
#include "player.hpp"
#include "comp.hpp"
#include "enemy.hpp"
#include "entity.hpp"

using std::vector;
using std::pair;
using std::shared_ptr;
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


#endif