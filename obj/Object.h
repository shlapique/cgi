#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Planesets.h"


class Object 
{
    public:
        Object();
        void create_pyramid(double a, double h);
        void create_cube(double a);
        void create_tetra(double a);

        std::vector <Point> vertex;
        std::vector <V4> planeset;
        std::vector <std::vector <int>> connections;
};

#endif
