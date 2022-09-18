#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>


struct Point
{
    int x;
    int y;
};

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

void draw_parabola(SDL_Renderer *renderer, int size_x, int size_y, Point origin, double prec)
{
    draw_circle(renderer, origin.x + (1 / prec), origin.y, 4);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    double f;
    int x, y;
    int prevp_x = origin.x - size_x;
    f = std::pow(prevp_x * prec, 2);
    y = (int)std::round(f * (1 / prec));
    int prevp_y = size_y - origin.y - y;

    for(int i = origin.x - size_x + 1; i < size_x; ++i)
    {
        f = std::pow(i * prec, 2);

        y = (int)std::round(f * (1 / prec));
        y = size_y - origin.y - y;
        x = origin.x + i;
        SDL_RenderDrawPoint(renderer, x, y);
        SDL_RenderDrawLine(renderer, prevp_x, prevp_y, x, y);
        prevp_x = x;
        prevp_y = y;
    }
}

void draw_sin(SDL_Renderer *renderer, int size_x, int size_y, Point origin, double prec)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    double f;
    int x, y;
    int prevp_x = origin.x - size_x;
    f = std::sin(prevp_x * prec);
    y = (int)std::round(f * (1 / prec));
    int prevp_y = size_y - origin.y - y;

    for(int i = origin.x - size_x + 1; i < size_x; ++i)
    {
        f = std::sin(i * prec);

        y = (int)std::round(f * (1 / prec));
        y = size_y - origin.y - y;
        x = origin.x + i;
        SDL_RenderDrawPoint(renderer, x, y);
        SDL_RenderDrawLine(renderer, prevp_x, prevp_y, x, y);
        prevp_x = x;
        prevp_y = y;
    }
}

// 21
void draw_flower(SDL_Renderer *renderer, int size_x, int size_y, Point origin, double prec, double a)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    double f, xr, yr;
    int x, y;
    f = a * std::cos(((origin.x - size_x) * prec) * 7); // ro
    xr = f * std::cos((origin.x - size_x) * prec);
    yr = f * std::sin((origin.x - size_x) * prec);
    y = (int)std::round(yr * (1 / prec));
    x = (int)std::round(xr * (1 / prec));
    int prevp_x = origin.x + x;
    int prevp_y = size_y - origin.y - y;

    for(int i = origin.x - size_x + 1; i < size_x; ++i)
    {
        f = a * std::cos((i * prec) * 7); // ro
        xr = f * std::cos(i * prec);
        yr = f * std::sin(i * prec);
        x = (int)std::round(xr * (1 / prec));
        x = origin.x + x;
        y = (int)std::round(yr * (1 / prec));
        y = size_y - origin.y - y;
        SDL_RenderDrawPoint(renderer, x, y);
        SDL_RenderDrawLine(renderer, prevp_x, prevp_y, x, y);
        prevp_x = x;
        prevp_y = y;
    }
}

void draw_graph(SDL_Renderer *renderer, int size_x, int size_y, Point origin, double prec, double a)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    double f, xr, yr;
    int x, y;
    f = a * (1 - std::cos((origin.x - size_x) * prec)); // ro
    xr = f * std::cos((origin.x - size_x) * prec);
    yr = f * std::sin((origin.x - size_x) * prec);
    y = (int)std::round(yr * (1 / prec));
    x = (int)std::round(xr * (1 / prec));
    int prevp_x = origin.x + x;
    int prevp_y = size_y - origin.y - y;

    for(int i = origin.x - size_x + 1; i < size_x; ++i)
    {
        f = a * (1 - std::cos(i * prec)); // ro
        xr = f * std::cos(i * prec);
        yr = f * std::sin(i * prec);
        x = (int)std::round(xr * (1 / prec));
        x = origin.x + x;
        y = (int)std::round(yr * (1 / prec));
        y = size_y - origin.y - y;
        SDL_RenderDrawPoint(renderer, x, y);
        SDL_RenderDrawLine(renderer, prevp_x, prevp_y, x, y);
        prevp_x = x;
        prevp_y = y;
    }
}

Point draw_coords(SDL_Renderer *renderer, int size_x, int size_y)
{
    Point point;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, size_x / 2, 0, size_x / 2, size_y);
    SDL_RenderDrawLine(renderer, 0, size_y / 2, size_x, size_y / 2);
    point.x = size_x / 2;
    point.y = size_y / 2; 
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
                Point point = draw_coords(renderer, size_x, size_y);
                draw_parabola(renderer, size_x, size_y, point, (1 / (mult * 100)));
                // draw_graph(renderer, size_x, size_y, point, 0.1 * mult, a);

                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) 
                {
                    if (event.type == SDL_QUIT) 
                    {
                        done = SDL_TRUE;
                    }
                
                    if (event.type == SDL_MOUSEWHEEL)
                    {
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
                    }
                }
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
