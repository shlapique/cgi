#ifndef __MATH_H__
#define __MATH_H__

#include <cmath>
#include "Core.h"
#include <vector>
#include <cstdio>

double scalar_mult(V4, V4);
Point polar_to_dec(double, double);
V4 plane_equation(Point, Point, Point);

//in 2d
double dist_flat(Point, Point);

//in 3d
double dist_stereo(Point, Point);

Point real_point(Point, Point);
std::vector <Edge> edges_to_render(std::vector <V4>, std::vector <std::vector <int>>, std::vector <Point>);

#endif
