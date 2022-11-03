#ifndef __PLANESETS_H__
#define __PLANESETS_H__

#include <vector>
#include "Core.h"
#include "Math.h"

std::vector <V4> planeset_pyr(std::vector <Point> vertex);
std::vector <V4> planeset_cube(std::vector <Point> vertex);
std::vector <V4> planeset_tetra(std::vector <Point> vertex);

#endif
