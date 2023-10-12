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

struct Solution
{
    int x;
    int y;
    double X;
    /* double f; */
};


Point polar_to_dec(double ro, double phi)
{
    Point point;
    point.rx = ro * std::cos(phi);
    point.ry = ro * std::sin(phi);
    return point;
}

void render_text(SDL_Renderer *renderer, int x, int y, char *text,
                TTF_Font *font, SDL_Rect *rect, SDL_Color *color)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    surface = TTF_RenderText_Solid(font, text, *color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->x = x;
    rect->y = y;
    rect->w = surface->w;
    rect->h = surface->h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
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


/* void draw(SDL_Renderer *renderer, std::vector <std::vector <int>> vec, std::vector <Solution> sol, int size_x, int size_y, Color color, TTF_Font * font, SDL_Color * text_color) */
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
    /* printf("SIZE OF SOLUTIONS IS %ld \n", sol.size()); */
    /* for(size_t i = 0; i < sol.size(); ++i) */
    /* { */
    /*     char text[200]; */
    /*     SDL_Rect rect; */
    /*     sprintf(text, "%f", sol[i].X); */ 
    /*     render_text(renderer, sol[i].x, sol[i].y + 2, text, font, &rect, text_color); */
    /* } */
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


std::vector <std::vector <int>> asin(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        f = std::asin(i * prec);
        y = size_y - origin.y - (int)std::round(f * (1 / prec));
        x = origin.x + i;
        vec.push_back({x , y});
    }
    return vec;
}


std::vector <std::vector <int>> f2(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        f = 2 + std::sin(i * prec);
        y = size_y - origin.y - (int)std::round(f * (1 / prec));
        x = origin.x + i;
        vec.push_back({x , y});
    }
    return vec;
}


std::vector <std::vector <int>> f1(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        /* x - cos(y) = 2 */
        f = std::acos(i * prec - 2);
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

std::vector <std::vector <int>> cardioid(int size_x, int size_y, Point origin, double prec, double a)
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

std::vector <std::vector <int>> ellipse(int size_x, int size_y, Point origin, double prec, double a, double b)
{
    std::vector <std::vector <int>> vec;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        // f = a * (1 - std::cos(i * prec)); // ro
        x = origin.x + (int)std::round(a * std::cos(i * prec));
        y = size_y - origin.y - (int)std::round(b * std::sin(i * prec));
        vec.push_back({x , y});
    }
    return vec;
}

std::vector <std::vector <int>> exp(int size_x, int size_y, Point origin, double prec, std::vector <Solution> &sol)
{
    /* e^{x} - x^{3} + 3x^{2} - 2x - 3 = 0 */
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        /* f = std::pow(i * prec, 3) + std::pow(i * prec, 2) - 2 * i * prec - 1; */
        f = std::exp(i * prec) - std::pow(i * prec, 3) + 3 * std::pow(i * prec, 2) - 2 * i * prec - 3;
        /* f = std::exp(i * prec); */
        printf("f = %f \n", f);
        printf("prec = %f \n", prec);

        x = origin.x + i;
        y = size_y - origin.y - (int)std::round(f * (1 / prec));
        if(x >= 0 && y >= 0)

        {
            if(x < 2000 && y < 2000)
            {
                /* find soulutions */
                /* if(std::abs((int)((f - prec) / prec)) <= prec) */
                if(std::abs((int)(f / prec) * prec) <= prec)
                {
                    printf("FOUND ZERO ! f = %f, round(f / prec) * prec) = %f", f, std::round(f / prec) * prec);
                    Solution s = {x, y, i * prec};
                    vec.push_back({x, y});
                    sol.push_back(s);
                }
                else
                    vec.push_back({x , y});
            }
        }
    }
    return vec;
}

Point origin(int size_x, int size_y)
{
    Point point = {size_x / 2, size_y / 2};
    return point;
}


void draw_coords(SDL_Renderer *renderer, int size_x, int size_y, double prec, Color color, TTF_Font * font, SDL_Color * text_color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);

    int ii = -1;
    for(int i = 1; i < size_x / 2; i += 1 / prec)
    {
        ii++;
        draw_circle(renderer, origin(size_x, size_y).x + i, origin(size_x, size_y).y, 4);
        draw_circle(renderer, origin(size_x, size_y).x - i, origin(size_x, size_y).y, 4);
        char text[200];
        char ttext[200];
        SDL_Rect rect;
        sprintf(text, "%d", ii); 
        sprintf(ttext, "%d", -ii); 
        render_text(renderer, origin(size_x, size_y).x + i, origin(size_x, size_y).y + 2, text, font, &rect, text_color);
        render_text(renderer, origin(size_x, size_y).x - i, origin(size_x, size_y).y + 2, ttext, font, &rect, text_color);
    }

    int jj = -1;
    for(int j = 1; j < size_y / 2; j += 1 / prec)
    {
        jj++;
        draw_circle(renderer, origin(size_x, size_y).x, origin(size_x, size_y).y - j, 4);
        draw_circle(renderer, origin(size_x, size_y).x, origin(size_x, size_y).y + j, 4);
        char text[200];
        char ttext[200];
        SDL_Rect rect;
        sprintf(text, "%d", jj); 
        sprintf(ttext, "%d", -jj); 
        render_text(renderer, origin(size_x, size_y).x - 2, origin(size_x, size_y).y + j, text, font, &rect, text_color);
        render_text(renderer, origin(size_x, size_y).x - 2, origin(size_x, size_y).y - j, ttext, font, &rect, text_color);
    }

    SDL_RenderDrawLine(renderer, size_x / 2, 0, size_x / 2, size_y);
    SDL_RenderDrawLine(renderer, 0, size_y / 2, size_x, size_y / 2);
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


int main(int argc, char *argv[])
{
    int size_x = 640;
    int size_y = 480; 
    double a = 10;
    double b = 10;
    double mult = 1;
    Color color = {231, 222, 111};
    Color coord_color = {2, 0, 200};

    char text[200];
    SDL_Color text_color{155, 38, 182, 255};
    TTF_Init();
    TTF_Font * font = TTF_OpenFont("./font/SweetsSmile.ttf", 23);
    if (font == NULL)
    {
        printf("error ! font not found\n");
        exit(EXIT_FAILURE);
    }

    /* printf("ENTER a parameter = "); */
    /* scanf("%lf", &a); */
    //scanf("%lf", &b);
    //
    
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
                Point point = origin(size_x, size_y);
                draw_coords(renderer, size_x, size_y, 1 / (mult * 100), coord_color, font, &text_color);

                ////
                /* draw(renderer, cardioid(size_x, size_y, point, (1 / (mult * 100)), a), size_x, size_y, color); */
                /* draw(renderer, flower(size_x, size_y, point, (1 / (mult * 100)), a), size_x, size_y, color); */
                /* draw(renderer, parabola(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color); */
                /* draw(renderer, sin(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color); */
                /* draw(renderer, asin(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color); */
                draw(renderer, f1(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color);
                draw(renderer, f2(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color);
                /* draw(renderer, ellipse(size_x, size_y, point, (1 / (mult * 100)), a, b), size_x, size_y, color); */

                /* std::vector <Solution> solutions; */
                /* std::vector <std::vector <int>> vec = exp(size_x, size_y, point, (1 / (mult * 100)), solutions); */
                /* draw(renderer, vec, solutions, size_x, size_y, color, font, &text_color); */ 
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
