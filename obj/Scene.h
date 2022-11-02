#ifndef __SCENE_H__
#define __SCENE_H__

#include "Core.h"
#include "Math.h"

class Scene
{
    public:
        Scene();
        Point rotate_x(Point, double, int);
        Point rotate_y(Point, double, int);
        Point rotate_z(Point, double, int);
        void rotate(Axis, std::vector <Point> &obj, double, int, double);

        //perspective projection
        Point central_projection(Point, double);

        //perspective projection for an object
        void central_projection(std::vector <Point> &obj, Point, double);

        //perspective projection for array of edges
        void central_projection(std::vector <Edge> &edges, Point, double);

        void isometric_projection(std::vector <Point> &obj, Point);

        //isomtric projection for array of edges
        void isometric_projection(std::vector <Edge> &edges, Point);

        std::vector <V4> visibility(std::vector <V4>);

        /*
        //midpoint circle algorithm
        void draw_circle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);
        void draw_segment(SDL_Renderer *renderer, Point a, Point b, Color color);
        void draw_obj(SDL_Renderer *renderer, std::vector <Edge> edges, Color color);
        */

    private:
        //SDL_Renderer *renderer = NULL; 
        double mult;
        int dir = 1; //default for my SC is clockwise (oZ looks away from me)
        Axis axis = X; // to change axis of rotation
        double scale = 1; // size of obj in "times"
        double scale_time = 1;
        /// default distance (k) from proj to screen
        double k = 600;
        Color color = {231, 222, 111};
};

#endif
