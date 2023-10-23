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
void draw_circle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius, Color color)
{
   SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
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

void draw_tabular(SDL_Renderer *renderer, std::vector <std::vector <int>> vec, int size_x, int size_y, Color color)
{
    for(size_t i = 0; i < vec.size(); ++i)
        draw_circle(renderer, vec[i][0], vec[i][1], 7, color);
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


// std::vector <std::vector <int>> f1(int size_x, int size_y, Point origin, double prec)
// {
//     std::vector <std::vector <int>> vec;
//     double f;
//     int x, y;
//     for(int i = origin.x - size_x; i < size_x; ++i)
//     {
//         /* x - cos(y) = 2 */
//         f = std::acos(i * prec - 2);
//         y = size_y - origin.y - (int)std::round(f * (1 / prec));
//         x = origin.x + i;
//         vec.push_back({x , y});
//     }
//     return vec;
// }

std::vector <std::vector <int>> f1(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.y - size_y; i < size_y; ++i)
    {
        /* x - cos(y) = 2 */
        // f = std::acos(i * prec - 2);
        f = 2 + std::cos(i * prec);
        x = size_x - origin.x + (int)std::round(f * (1 / prec));
        y = origin.y + i;
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

std::vector <std::vector <int>> mnk1(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        /* f = 0.471276 + 0.317708 * i * prec; */
        f = 1.57079 - 0.325699 * i * prec;
        x = origin.x + i;
        y = size_y - origin.y - (int)std::round(f * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}

std::vector <std::vector <int>> mnk2(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        /* f = 0.129443 + 0.619325 * i * prec - 0.0354844 * std::pow(i * prec, 2); */
        f = 1.57079 - 0.325699 * i * prec + 2.23214 * 0.0000001 * std::pow(i * prec, 2);
        x = origin.x + i;
        y = size_y - origin.y - (int)std::round(f * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}

/* for 4.1 its euler */
std::vector <std::vector <int>> f_tabular(int size_x, int size_y, Point origin, double prec)
{
    /* std::vector <std::vector <double>> tabular = {{0, 0}, {1.7, 1.3038}, {3.4, 1.8439}, {5.1, 2.2583}, */
    /*     {6.8, 2.6077}, {8.5, 2.9155}}; */

    /* std::vector <std::vector <double>> tabular = {{-5, 2.9442}, {-3, 2.8198}, {-1, 2.3562}, {1, 0.78540}, */ 
    /*     {3, 0.32175}, {5, 0.19740}}; */

    std::vector <std::vector <double>> tabular = 
    {{0, 2},
	{0.1, 2.02},
	{0.2, 2.0598},
	{0.3, 2.11857},
	{0.4, 2.19487},
	{0.5, 2.28656},
	{0.6, 2.39085},
	{0.7, 2.50429},
	{0.8, 2.62282},
	{0.9, 2.74188},
	{1, 2.85653}};

    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = 0; i < (int)tabular.size(); ++i)
    {
        x = size_x - origin.x + (int)std::round(tabular[i][0] * (1 / prec));
        y = size_y - origin.y - (int)std::round(tabular[i][1] * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}

/* for 4.1 its euler */
std::vector <std::vector <int>> f_tabular_2(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <double>> tabular = 
	{{0, 2},
	{0.05, 2.005},
	{0.1, 2.01499},
	{0.15, 2.02991},
	{0.2, 2.04969},
	{0.25, 2.07418},
	{0.3, 2.10323},
	{0.35, 2.13663},
	{0.4, 2.17413},
	{0.45, 2.21546},
	{0.5, 2.26027},
	{0.55, 2.30821},
	{0.6, 2.35888},
	{0.65, 2.41183},
	{0.7, 2.46659},
	{0.75, 2.52265},
	{0.8, 2.57948},
	{0.85, 2.63651},
	{0.9, 2.69316},
	{0.95, 2.74885}};
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = 0; i < (int)tabular.size(); ++i)
    {
        x = size_x - origin.x + (int)std::round(tabular[i][0] * (1 / prec));
        y = size_y - origin.y - (int)std::round(tabular[i][1] * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}

std::vector <std::vector <int>> f_tabular_runge(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <double>> tabular = 
    {{0, 2},
    {0.1, 2.00998},
    {0.2, 2.0396},
    {0.3, 2.08797},
    {0.4, 2.15357},
    {0.5, 2.23429},
    {0.6, 2.32738},
    {0.7, 2.42957},
    {0.8, 2.53705},
    {0.9, 2.64563},
    {1, 2.75085}};

    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = 0; i < (int)tabular.size(); ++i)
    {
        x = size_x - origin.x + (int)std::round(tabular[i][0] * (1 / prec));
        y = size_y - origin.y - (int)std::round(tabular[i][1] * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}

std::vector <std::vector <int>> f_tabular_runge_2(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <double>> tabular = 
	{{0, 2},
	{0.05, 2.0025},
	{0.1, 2.00997},
	{0.15, 2.02237},
	{0.2, 2.0396},
	{0.25, 2.06152},
	{0.3, 2.08797},
	{0.35, 2.11874},
	{0.4, 2.15357},
	{0.45, 2.1922},
	{0.5, 2.23429},
	{0.55, 2.27948},
	{0.6, 2.32738},
	{0.65, 2.37757},
	{0.7, 2.42957},
	{0.75, 2.4829},
	{0.8, 2.53705},
	{0.85, 2.59147},
	{0.9, 2.64563},
	{0.95, 2.69894}};
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = 0; i < (int)tabular.size(); ++i)
    {
        x = size_x - origin.x + (int)std::round(tabular[i][0] * (1 / prec));
        y = size_y - origin.y - (int)std::round(tabular[i][1] * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}
std::vector <std::vector <int>> f_tabular_adams(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <double>> tabular = 
	{{0, 2},
	{0.1, 2.00998},
	{0.2, 2.0396},
	{0.3, 2.08797},
	{0.4, 2.15357},
	{0.5, 2.23428},
	{0.6, 2.32737},
	{0.7, 2.42952},
	{0.8, 2.53694},
	{0.9, 2.64544},
	{1, 2.75055}};


    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = 0; i < (int)tabular.size(); ++i)
    {
        x = size_x - origin.x + (int)std::round(tabular[i][0] * (1 / prec));
        y = size_y - origin.y - (int)std::round(tabular[i][1] * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}

std::vector <std::vector <int>> f_tabular_adams_2(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <double>> tabular = 
	{{0, 2},
	{0.05, 2.0025},
	{0.1, 2.00997},
	{0.15, 2.02237},
	{0.2, 2.0396},
	{0.25, 2.06152},
	{0.3, 2.08797},
	{0.35, 2.11874},
	{0.4, 2.15357},
	{0.45, 2.1922},
	{0.5, 2.23428},
	{0.55, 2.27948},
	{0.6, 2.32738},
	{0.65, 2.37756},
	{0.7, 2.42956},
	{0.75, 2.4829},
	{0.8, 2.53704},
	{0.85, 2.59146},
	{0.9, 2.64561},
	{0.95, 2.69892}};
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = 0; i < (int)tabular.size(); ++i)
    {
        x = size_x - origin.x + (int)std::round(tabular[i][0] * (1 / prec));
        y = size_y - origin.y - (int)std::round(tabular[i][1] * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}


std::vector <std::vector <int>> exact_solution_1(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        if(i * prec >= 0 && i * prec < 1)
        {
            f = std::exp(std::sin(i * prec)) + std::exp(-std::sin(i * prec));
            x = origin.x + i;
            y = size_y - origin.y - (int)std::round(f * (1 / prec));
            vec.push_back({x, y});
        }
    }
    return vec;
}

std::vector <std::vector <int>> f_tabular_shooting_1(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <double>> tabular = 
	{{0, 0.5},
	{0.1, 0.580178},
	{0.2, 0.67154},
	{0.3, 0.775371},
	{0.4, 0.893073},
	{0.5, 1.02618},
	{0.6, 1.17636},
	{0.7, 1.34543},
	{0.8, 1.53541},
	{0.9, 1.74846},
	{1, 1.987}};

    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = 0; i < (int)tabular.size(); ++i)
    {
        x = size_x - origin.x + (int)std::round(tabular[i][0] * (1 / prec));
        y = size_y - origin.y - (int)std::round(tabular[i][1] * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}

std::vector <std::vector <int>> f_tabular_shooting_2(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <double>> tabular = 
	{{0, 0.5},
	{0.05, 0.538767},
	{0.1, 0.580178},
	{0.15, 0.624383},
	{0.2, 0.67154},
	{0.25, 0.721812},
	{0.3, 0.775371},
	{0.35, 0.832396},
	{0.4, 0.893073},
	{0.45, 0.957599},
	{0.5, 1.02618},
	{0.55, 1.09902},
	{0.6, 1.17636},
	{0.65, 1.25841},
	{0.7, 1.34543},
	{0.75, 1.43768},
	{0.8, 1.53541},
	{0.85, 1.63891},
	{0.9, 1.74846},
	{0.95, 1.86439},
	{1, 1.987}};


    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = 0; i < (int)tabular.size(); ++i)
    {
        x = size_x - origin.x + (int)std::round(tabular[i][0] * (1 / prec));
        y = size_y - origin.y - (int)std::round(tabular[i][1] * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}

std::vector <std::vector <int>> f_tabular_fd_1(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <double>> tabular = 
	{{0, 0.5},
	{0.1, 0.580142},
	{0.2, 0.671476},
	{0.3, 0.775286},
	{0.4, 0.892976},
	{0.5, 1.02608},
	{0.6, 1.17626},
	{0.7, 1.34535},
	{0.8, 1.53534},
	{0.9, 1.74843},
	{1, 1.987}};

    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = 0; i < (int)tabular.size(); ++i)
    {
        x = size_x - origin.x + (int)std::round(tabular[i][0] * (1 / prec));
        y = size_y - origin.y - (int)std::round(tabular[i][1] * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}

std::vector <std::vector <int>> f_tabular_fd_2(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <double>> tabular = 
	{{0, 0.5},
	{0.05, 0.538762},
	{0.1, 0.580169},
	{0.15, 0.624371},
	{0.2, 0.671524},
	{0.25, 0.721793},
	{0.3, 0.77535},
	{0.35, 0.832373},
	{0.4, 0.893049},
	{0.45, 0.957574},
	{0.5, 1.02615},
	{0.55, 1.099},
	{0.6, 1.17633},
	{0.65, 1.25839},
	{0.7, 1.34541},
	{0.75, 1.43766},
	{0.8, 1.53539},
	{0.85, 1.63889},
	{0.9, 1.74846},
	{0.95, 1.86438},
	{1, 1.987}};


    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = 0; i < (int)tabular.size(); ++i)
    {
        x = size_x - origin.x + (int)std::round(tabular[i][0] * (1 / prec));
        y = size_y - origin.y - (int)std::round(tabular[i][1] * (1 / prec));
        vec.push_back({x, y});
    }
    return vec;
}

std::vector <std::vector <int>> exact_solution_2(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        if(i * prec > 0 && i * prec < 1)
        {
            f = std::exp(i * prec) - 1 + 1 / (std::exp(i * prec) + 1);
            x = origin.x + i;
            y = size_y - origin.y - (int)std::round(f * (1 / prec));
            vec.push_back({x, y});
        }
    }
    return vec;
}

std::vector <std::vector <int>> qspline1(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        if(i * prec >= -3 && i * prec < -1)
        {
            f = 2.819800 + -0.047003 * (i * prec + 3.000000) + 0.000000 * std::pow(i * prec + 3.000000, 2) + -0.046199*std::pow(i * prec + 3.000000, 3);
            x = origin.x + i;
            y = size_y - origin.y - (int)std::round(f * (1 / prec));
            vec.push_back({x, y});
        }
    }
    return vec;
}

std::vector <std::vector <int>> qspline2(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        if(i * prec >= -1 && i * prec < 1)
        {
            f = 2.356200 + -0.601395 * (i * prec - -1.000000) + -0.277196 * std::pow(i * prec - -1.000000, 2) + 0.092597 * std::pow(i * prec - -1.000000, 3);
            x = origin.x + i;
            y = size_y - origin.y - (int)std::round(f * (1 / prec));
            vec.push_back({x, y});
        }
    }
    return vec;
}
std::vector <std::vector <int>> qspline3(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        if(i * prec >= 1 && i * prec <= 3)
        {
            f = 0.785400 + -0.599019 * (i * prec - 1.000000) + 0.278384 * std::pow(i * prec - 1.000000, 2) + -0.047394 * std::pow(i * prec - 1.000000, 3);
            x = origin.x + i;
            y = size_y - origin.y - (int)std::round(f * (1 / prec));
            vec.push_back({x, y});
        }
    }
    return vec;
}
std::vector <std::vector <int>> qspline4(int size_x, int size_y, Point origin, double prec)
{
    std::vector <std::vector <int>> vec;
    double f;
    int x, y;
    for(int i = origin.x - size_x; i < size_x; ++i)
    {
        if(i * prec >= 3 && i * prec <= 5)
        {
            f = 0.321750 + -0.054205 * (i * prec - 3.000000) + -0.005977 * std::pow(i * prec - 3.000000, 2) + 0.000996 * std::pow(i * prec - 3.000000, 3);
            x = origin.x + i;
            y = size_y - origin.y - (int)std::round(f * (1 / prec));
            vec.push_back({x, y});
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
        draw_circle(renderer, origin(size_x, size_y).x + i, origin(size_x, size_y).y, 4, {255, 0, 0});
        draw_circle(renderer, origin(size_x, size_y).x - i, origin(size_x, size_y).y, 4, {255, 0, 0});
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
        draw_circle(renderer, origin(size_x, size_y).x, origin(size_x, size_y).y - j, 4, {255, 0, 0});
        draw_circle(renderer, origin(size_x, size_y).x, origin(size_x, size_y).y + j, 4, {255, 0, 0});
        char text[200];
        char ttext[200];
        SDL_Rect rect;
        sprintf(text, "%d", -jj); 
        sprintf(ttext, "%d", jj); 
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

    Color color_f1 = {102, 232, 237};
    Color color_f2 = {237, 188, 102};

    Color color_mnk1 = {237, 102, 178};
    Color color_mnk2 = {115, 89, 176};
    Color color_tabular = {162, 229, 112};

    Color color_s1 = {255, 255, 255};
    Color color_s2 = {106, 147, 208};
    Color color_s3 = {55, 139, 64};
    Color color_s4 = {94, 64, 160};
    
    Color eu_color = {102, 232, 237}; // euler method
    Color exact_color = {237, 188, 102}; // exact solution
    Color runge_color = {255, 0, 0}; // exact solution
    Color adams_color = {0, 255, 0}; // exact solution

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

                /* draw(renderer, f1(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color_f1); */
                /* draw(renderer, f2(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color_f2); */

                /* draw(renderer, qspline1(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color_s1); */
                /* draw(renderer, qspline2(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color_s2); */
                /* draw(renderer, qspline3(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color_s3); */
                /* draw(renderer, qspline4(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color_s4); */

                /* draw(renderer, mnk1(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color_mnk1); */
                /* draw(renderer, mnk2(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color_mnk2); */
                /* draw_tabular(renderer, f_tabular(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, color_tabular); */


                /* ------------- */
                /* 4.1 */
                /* draw(renderer, f_tabular(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, eu_color); */
                /* draw(renderer, f_tabular_runge(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, runge_color); */
                /* draw(renderer, f_tabular_adams(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, adams_color); */

                /* draw(renderer, f_tabular_2(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, eu_color); */
                /* draw(renderer, f_tabular_runge_2(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, runge_color); */
                /* draw(renderer, f_tabular_adams_2(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, adams_color); */

                /* draw(renderer, exact_solution_1(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, exact_color); */
                /* ------------- */

                /* ------------- */
                /* 4.2 */
                /* draw(renderer, f_tabular_shooting_1(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, eu_color); */
                /* draw(renderer, f_tabular_fd_1(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, runge_color); */

                /* draw(renderer, f_tabular_shooting_2(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, eu_color); */
                /* draw(renderer, f_tabular_fd_2(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, runge_color); */

                /* draw(renderer, exact_solution_2(size_x, size_y, point, (1 / (mult * 100))), size_x, size_y, exact_color); */
                /* ------------- */


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
