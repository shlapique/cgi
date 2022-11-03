#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include "Core.h"
#include "Math.h"

class Object 
{
    public:
        Object();
        void create_pyramid(double a, double h);
        void create_cube(double a);
        void create_tetra(double a);

        std::vector <Point> vertex;
        std::vector <std::vector <int>> planeset;
        std::vector <std::vector <int>> connections;
};

#endif
