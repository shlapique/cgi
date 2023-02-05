#ifndef __MATH_H__
#define __MATH_H__

#include <cmath>
#include "Core.h"
#include <vector>
#include <cstdio>

double scalar_mult(V4, V4);
double scalar_mult(V3, V3);
V3 scalar_mult(V3, double);
V3 scalar_mult(V4, double);


Point polar_to_dec(double, double);
V4 plane_equation(Point, Point, Point);
std::vector <V4> get_planeset(std::vector <Point>, std::vector <std::vector <int>>);

//in 2d
double dist_flat(Point, Point);

//in 3d
double dist_stereo(Point, Point);

V3 scalar_mult(V3, double);
V3 normalize(V3);
V3 normalize(V4);
    
Point real_point(Point, Point);
Point find_origin(int, int, double);
std::vector <Edge> edges_to_render(std::vector <V4>, std::vector <std::vector <int>>, std::vector <Point>);
std::vector <std::vector <Point>> tri_to_render(std::vector <V4>, std::vector <std::vector <int>>, std::vector <Point>);
std::vector <V4> visibility(std::vector <V4>);
std::vector <std::vector <Point>> delaunay(std::vector <Point>);
std::vector <double> brightness(std::vector <V4>, std::vector <std::vector <Point>>);

#endif
