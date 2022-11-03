#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

#include "Core.h"
#include "Math.h"
#include "Scene.h"
#include "Object.h"
 
void isometric_projection(std::vector <Point> &obj, Point origin)
{
    for(size_t i = 0; i < obj.size(); ++i)
    {
        obj[i] = real_point(origin, obj[i]);
    }
}

//isomtric projection for array of edges
void isometric_projection(std::vector <Edge> &edges, Point origin)
{
    for(size_t i = 0; i < edges.size(); ++i)
    {
        edges[i] = {real_point(origin, edges[i].a), real_point(origin, edges[i].b)};
    }
}


int main(int argc, char *argv[])
{
    int size_x = 640;
    int size_y = 480; 
    double mult = 0;
    int dir = 1; //default for my SC is clockwise (oZ looks away from me)
    Axis axis = X; // to change axis of rotation
    double scale = 1; // size of obj in "times"
    double scale_time = 1;

    /// default distance (k) from proj to screen
    double k = 600;

    // for pyramid
    ///
    double a = 100;
    double h = 600;
    ///

    Color color = {231, 222, 111};
    //Color coord_color = {2, 0, 200};
    
    if (SDL_Init(SDL_INIT_VIDEO) == 0) 
    {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;

        if (SDL_CreateWindowAndRenderer(size_x, size_y, SDL_WINDOW_RESIZABLE, &window, &renderer) == 0) 
        {
            SDL_bool done = SDL_FALSE;
            //+++++++++++++++
            Scene scene(renderer);
            scene.create_pyramid(100, 600);
            //scene.create_cube(100);
            //scene.create_tetra(100);
            //+++++++++++++++
            while (!done) 
            {
                SDL_Event event;
                int x, y;
                Uint32 buttons;
                SDL_PumpEvents();  // make sure we have the latest mouse state.
                buttons = SDL_GetMouseState(&x, &y);
                SDL_GetWindowSize(window, &size_x, &size_y);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer); 

                //find center (origin)
                Point origin = find_origin(size_x, size_y, k);

                //////
                scene.rotate(axis, mult, dir, k);
                scene.central_projection(origin, k);
                //scene.isometric_projection(origin);
                scene.draw(color);
                mult = 0;
                //////

                SDL_RenderPresent(renderer);
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                        case SDL_QUIT:
                            done = SDL_TRUE;
                            break;
                        case SDL_MOUSEWHEEL:
                            if(event.wheel.y > 0) // scroll up
                            {
                                if(scale_time + 0.1 <= 3)
                                {
                                    scale += 0.1;
                                    scale_time += 0.1;
                                    scene.transform(scale);
                                    //printf("SCALE = %f, scale_time = %f\n", scale, scale_time);
                                    scale = 1;
                                }
                            }
                            else if(event.wheel.y < 0) // scroll down
                            {
                                if(scale_time - 0.1 >= 0.1)
                                {
                                    scale -= 0.1;
                                    scale_time -= 0.1;
                                    scene.transform(scale);
                                    //printf("SCALE = %f, scale_time = %f\n", scale, scale_time);
                                    scale = 1;
                                }
                                else
                                {
                                    //printf("SCALE arg is < 0.1!!!\n");
                                }
                            }
                            break;                            

                        case SDL_KEYDOWN:
                            switch(event.key.keysym.sym)
                            {
                                // SDLK_UP
                                case SDLK_k:
                                    dir = 1;
                                    axis = X;
                                    mult += 0.05;
                                    break;
                                
                                // SDLK_DOWN
                                case SDLK_j:
                                    dir = -1;
                                    axis = X;
                                    mult += 0.05;
                                    break;

                                // SDLK_LEFT
                                case SDLK_h:
                                    dir = 1;
                                    axis = Y;
                                    mult += 0.05;
                                    break;

                                // SDLK_RIGHT
                                case SDLK_l:
                                    dir = -1;
                                    axis = Y;
                                    mult += 0.05;
                                    break;

                                case SDLK_u:
                                    dir = 1;
                                    axis = Z;
                                    mult += 0.05;
                                    break;

                                case SDLK_d:
                                    dir = -1;
                                    axis = Z;
                                    mult += 0.05;
                                    break;

                                default:
                                    break;
                            }
                    }
                }
                SDL_Delay(20);
            }
        }

        if (renderer) 
        {
            SDL_DestroyRenderer(renderer);
        }
        if (window) 
        {
            SDL_DestroyWindow(window);
        }
    }

    SDL_Quit();
    return 0;
}
