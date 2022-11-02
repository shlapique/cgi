#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Scene.h"
#include "Planesets.h"


class Object : public Scene
{
    public:
        Object();
        void create_pyramid(double a, double h);

    private:
        std::vector <Point> vertex = {}; 
        std::vector <V4> planeset;
        std::vector <std::vector <int>> connections;
};

#endif
