#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <vector>


struct Point
{
    int x;
    int y;
    double rx;
    double ry;
};


struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};


Point polar_to_dec(double ro, double phi)
{
    Point point;
    point.rx = ro * std::cos(phi);
    point.ry = ro * std::sin(phi);
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


void draw(SDL_Renderer *renderer, std::vector <std::vector <int>> vec, int size_x, int size_y, Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);

    int prevx = vec[0][0];
    int prevy = vec[0][1];
    for(size_t i = 0; i < vec.size(); ++i)
    {
        SDL_RenderDrawPoint(renderer, vec[i][0], vec[i][1]);
        SDL_RenderDrawLine(renderer, prevx, prevy, vec[i][0], vec[i][1]);
        prevx = vec[i][0];
        prevy = vec[i][1];
    }
}


std::vector <std::vector <int>> parabola(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        f = std::pow(i * prec, 2);
        x = origin.x + i;
        y = size_y - origin.y - (int)std::round(f * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}


std::vector <std::vector <int>> sin(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        f = std::sin(i * prec);
        y = size_y - origin.y - (int)std::round(f * (1 / prec));
        x = origin.x + i;
        vec.push_back({x , y});
    }
    return vec;
}


// 21
std::vector <std::vector <int>> flower(int size_x, int size_y, Point origin, double prec, double a)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        f = a * std::cos((i * prec) * 7); // ro
        x = origin.x + (int)std::round(polar_to_dec(f, i * prec).rx * (1 / prec));
        y = size_y - origin.y - (int)std::round(polar_to_dec(f, i * prec).ry * (1 / prec));
        vec.push_back({x , y});
    }
    return vec;
}

std::vector <std::vector <int>> graph(int size_x, int size_y, Point origin, double prec, double a)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        f = a * (1 - std::cos(i * prec)); // ro
        x = origin.x + (int)std::round(polar_to_dec(f, i * prec).rx * (1 / prec));
        y = size_y - origin.y - (int)std::round(polar_to_dec(f, i * prec).ry * (1 / prec));
        vec.push_back({x , y});
    }
    return vec;
}

Point draw_coords(SDL_Renderer *renderer, int size_x, int size_y, double prec)
{
    Point point;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, size_x / 2, 0, size_x / 2, size_y);
    SDL_RenderDrawLine(renderer, 0, size_y / 2, size_x, size_y / 2);
    point.x = size_x / 2;
    point.y = size_y / 2; 
    draw_circle(renderer, point.x + (1 / prec), point.y, 4);
    return point;
}


double dist(int x1, int y1, int x2, int y2)
{
    double a = std::abs(x2 - x1);
    double b = std::abs(y2 - y1);
    double result = std::sqrt(std::pow(a, 2) + std::pow(b, 2));
    return result;
}

void draw_triangle(SDL_Renderer *renderer, int x, int y, int ts)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, x - ts / 2, y + ts / 2, x, y - ts / 2);
    SDL_RenderDrawLine(renderer, x, y - ts / 2, x + ts / 2, y + ts / 2);
    SDL_RenderDrawLine(renderer, x + ts / 2, y + ts / 2, x - ts / 2, y + ts / 2);
}

// later would be in "update screen" class
void draw_triangle(SDL_Renderer *renderer, int x, int y, int ts, unsigned int mult)
{
    ts *= mult;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, x - ts / 2, y + ts / 2, x, y - ts / 2);
    SDL_RenderDrawLine(renderer, x, y - ts / 2, x + ts / 2, y + ts / 2);
    SDL_RenderDrawLine(renderer, x + ts / 2, y + ts / 2, x - ts / 2, y + ts / 2);
}

int main(int argc, char *argv[])
{
    int size_x = 640;
    int size_y = 480; 
    double a = 10;
    double mult = 1;
    Color color = {231, 222, 111};

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

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  
                Point point = draw_coords(renderer, size_x, size_y, 1 / (mult * 100));

                ////
                draw(renderer, graph(size_x, size_y, point, (1 / (mult * 100)), a), size_x, size_y, color);
                draw(renderer, flower(size_x, size_y, point, (1 / (mult * 100)), a), size_x, size_y, color);
                draw(renderer, parabola(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color);
                draw(renderer, sin(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color);
                ////

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
                                mult += 0.1;
                                printf("Mult = %f\n", mult);
                            }

                            else if(event.wheel.y < 0) // scroll down
                            {
                                if(mult - 0.1 >= 0.1)
                                {
                                    mult -= 0.1;
                                    printf("Mult = %f\n", mult);
                                }
                                else
                                {
                                    printf("in minus field!\n");
                                }
                            }
                            break;                            
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
