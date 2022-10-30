#ifndef __MATH_H__
#define __MATH_H__

#include <cmath>
#include "Core.h"

double scalar_mult(V4, V4);
Point polar_to_dec(double, double);
V4 plane_equation(Point, Point, Point);

//in 2d
double dist_flat(Point, Point);

//in 3d
double dist_stereo(Point, Point);

#endif
