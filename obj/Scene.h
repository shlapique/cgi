#ifndef __SCENE_H__
#define __SCENE_H__



class Scene
{
    public:
        //midpoint circle algorithm
        void draw_circle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius)
        {
           SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
           const int32_t diameter = (radius * 2);

           int32_t x = (radius - 1);
           int32_t y = 0;
           int32_t tx = 1;
           int32_t ty = 1;
           int32_t error = (tx - diameter);

           while (x >= y)
           {
              //  Each of the following renders an octant of the circle
              SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
              SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
              SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
              SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
              SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
              SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
              SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
              SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

              if (error <= 0)
              {
                 ++y;
                 error += ty;
                 ty += 2;
              }

              if (error > 0)
              {
                 --x;
                 tx += 2;
                 error += (tx - diameter);
              }
            }
        }

        void draw_segment(SDL_Renderer *renderer, Point a, Point b, Color color)
        {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
        }


        void draw_obj(SDL_Renderer *renderer, std::vector <Edge> edges, Color color)
        {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
            for(size_t i = 0; i < edges.size(); ++i)
            {
                draw_segment(renderer, edges[i].a, edges[i].b, color);
            }
        }
}

#endif
