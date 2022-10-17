#include <SDL2/SDL.h>
#include <cmath>
#include <vector>


struct Point
{
    double x;
    double y;
    double z;
    double rx;
    double ry;
};


struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};


enum Axis 
{
    X, 
    Y, 
    Z
};

Point polar_to_dec(double ro, double phi)
{
    Point point;
    point.rx = ro * std::cos(phi);
    point.ry = ro * std::sin(phi);
    return point;
}

Point real_point(Point origin, Point a)
{
    Point point = {origin.x + a.x, origin.y - a.y, origin.z + a.z};
    return point;
}

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

void draw_cube(SDL_Renderer *renderer, Point cube[], Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    for(int i = 0; i < 3; ++i)
    {
        SDL_RenderDrawLine(renderer, cube[i].x, cube[i].y, cube[i + 1].x, cube[i + 1].y);
    }
    SDL_RenderDrawLine(renderer, cube[0].x, cube[0].y, cube[3].x, cube[3].y);

    for(int i = 4; i < 7; ++i)
    {
        SDL_RenderDrawLine(renderer, cube[i].x, cube[i].y, cube[i + 1].x, cube[i + 1].y);
    }
    SDL_RenderDrawLine(renderer, cube[4].x, cube[4].y, cube[7].x, cube[7].y);

    for(int i = 0; i < 4; ++i)
    {
        SDL_RenderDrawLine(renderer, cube[i].x, cube[i].y, cube[i + 4].x, cube[i + 4].y);
    }

}

//screen center
Point find_origin(int size_x, int size_y, double k)
{
    Point point = {std::round(size_x / 2), std::round(size_y / 2), k};
    return point;
}


// Where dir = 1 or -1
Point rotate_x(Point a, double phi, int dir)
{
    Point result;
    result.x = a.x;
    result.y = a.y * std::cos(phi) - dir * a.z * std::sin(phi);
    result.z = dir * a.y * std::sin(phi) + a.z * std::cos(phi);
    return result;
}

Point rotate_y(Point a, double phi, int dir)
{
    Point result;
    result.x = a.x * std::cos(phi) + dir * a.z * std::sin(phi);
    result.y = a.y;
    result.z = -dir * a.x * std::sin(phi) + a.z * std::cos(phi);
    return result;
}

Point rotate_z(Point a, double phi, int dir)
{
    Point result;
    result.x = a.x * std::cos(phi) - dir * a.y * std::sin(phi);
    result.y = dir * a.x * std::sin(phi) + a.y * std::cos(phi);
    result.z = a.z;
    return result;
}

//perspective projection
Point central_projection(Point a, int k)
{
    Point result;
    result.y = (k * a.y) / (a.z + k);
    result.x = (k * a.x) / (a.z + k);
    return result;
}

//in 2d
double dist_flat(Point p1, Point p2)
{
    double a = std::abs(p2.x - p1.x);
    double b = std::abs(p2.y - p1.y);
    double result = std::sqrt(std::pow(a, 2) + std::pow(b, 2));
    return result;
}

//in 3d
double dist_stereo(Point p1, Point p2)
{
    double a = std::abs(p2.x - p1.x);
    double b = std::abs(p2.y - p1.y);
    double c = std::abs(p2.z - p1.z);
    double result = std::sqrt(std::pow(a, 2) + std::pow(b, 2) + std::pow(c, 2));
    return result;
}

void draw_triangle(SDL_Renderer *renderer, int x, int y, int ts)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, x - ts / 2, y + ts / 2, x, y - ts / 2);
    SDL_RenderDrawLine(renderer, x, y - ts / 2, x + ts / 2, y + ts / 2);
    SDL_RenderDrawLine(renderer, x + ts / 2, y + ts / 2, x - ts / 2, y + ts / 2);
}


int main(int argc, char *argv[])
{
    int size_x = 640;
    int size_y = 480; 
    double mult = 0;
    int dir = 1; //default for my SC is clockwise (oZ looks away from me)
    Axis axis; // to change axis of rotation

    /// default distance (k) from proj to screen
    int k = 600;

    /// points for cube
    Point cube[8] = {{100, 100, -100}, {100, 100, 100}, {-100, 100, 100}, {-100, 100, -100},
                    {100, -100, -100}, {100, -100, 100}, {-100, -100, 100}, {-100, -100, -100}};
    
    Point pcube[8];
    Point rcube[8];
    ///

    //find center (origin)
    Point origin = find_origin(size_x, size_y, k);

    Color color = {231, 222, 111};
    //Color coord_color = {2, 0, 200};
    
    if (SDL_Init(SDL_INIT_VIDEO) == 0) 
    {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;

        if (SDL_CreateWindowAndRenderer(size_x, size_y, SDL_WINDOW_RESIZABLE, &window, &renderer) == 0) 
        {
            SDL_bool done = SDL_FALSE;
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


                ///
                for(int i = 0; i < 8; ++i)
                {
                    switch(axis)
                    {
                        case X:
                            cube[i] = rotate_x(cube[i], mult, dir); 
                            if(mult != 0)
                                printf("A%d = {%f, %f, %f}\n", i, cube[i].x, cube[i].y, cube[i].z);
                            break;

                        case Y:
                            cube[i] = rotate_y(cube[i], mult, dir); 
                            if(mult != 0)
                                printf("A%d = {%f, %f, %f}\n", i, cube[i].x, cube[i].y, cube[i].z);
                            break;

                        case Z:
                            cube[i] = rotate_z(cube[i], mult, dir); 
                            if(mult != 0)
                                printf("A%d = {%f, %f, %f}\n", i, cube[i].x, cube[i].y, cube[i].z);
                            break;
                        default:
                            break;
                    }
                }
                mult = 0;

                for(int i = 0; i < 8; ++i)
                {
                    pcube[i] = central_projection(cube[i], k);    
                }
                    
                for(int i = 0; i < 8; ++i)
                {
                    rcube[i] = real_point(origin, pcube[i]);    
                }
                draw_cube(renderer, rcube, color);
                ///


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
                                dir = 1;
                                axis = Z;
                                mult += 0.05;
                            }
                            else if(event.wheel.y < 0) // scroll down
                            {
                                dir = -1;
                                axis = Z;
                                mult += 0.05;
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
