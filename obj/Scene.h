#ifndef __SCENE_H__
#define __SCENE_H__

#include "Figure.h"

class Scene
{
    public:
        Scene()
        {
            


        }

        //midpoint circle algorithm
        void draw_circle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);
        void draw_segment(SDL_Renderer *renderer, Point a, Point b, Color color);
        void draw_obj(SDL_Renderer *renderer, std::vector <Edge> edges, Color color);

}

#endif
