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


// for canonical equation:
// ax + by + cz + d = 0;
struct V4
{
    double a; 
    double b; 
    double c; 
    double d; 
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

struct Edge
{
    Point a;
    Point b;
};


double scalar_mult(V4 vec1, V4 vec2)
{
    double result = vec1.a * vec2.a + vec1.b * vec2.b 
                    + vec1.c * vec2.c + vec1.d * vec2.d;
    return result;
}

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


void draw_obj(SDL_Renderer *renderer, std::vector <Edge> edges, Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    for(size_t i = 0; i < edges.size(); ++i)
    {
        draw_segment(renderer, edges[i].a, edges[i].b, color);
    }
}


V4 plane_equation(Point p1, Point p2, Point p3)
{
    // + - +
    p2 = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
    p3 = {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z};
    p1 = {-p1.x, -p1.y, -p1.z};

    Point minors= {(p2.y * p3.z - p3.y * p2.z), (p2.x * p3.z - p3.x * p2.z), (p2.x * p3.y - p3.x * p2.y)}; 
    V4 result = {minors.x, -minors.y, minors.z, p1.x * minors.x + (-1) * p1.y * minors.y + p1.z * minors.z};
    //printf("%f, %f, %f, %f", result.a, result.b, result.c, result.d);
    return result;
}


std::vector <V4> cube_planeset(std::vector <Point> cube)
{
    std::vector <V4> result(6);
    result[0] = plane_equation(cube[0], cube[3], cube[2]); // up
    result[1] = plane_equation(cube[1], cube[2], cube[5]); // back
    result[2] = plane_equation(cube[1], cube[5], cube[4]); // right
    result[3] = plane_equation(cube[4], cube[5], cube[6]); // bottom
    result[4] = plane_equation(cube[6], cube[2], cube[3]); // left
    result[5] = plane_equation(cube[0], cube[4], cube[7]); // front
    return result;
}

std::vector <V4> pyramid_planeset(std::vector <Point> obj)
{
    std::vector <V4> result(9);
    result[0] = plane_equation(obj[0], obj[1], obj[2]); // bottom 
    result[1] = plane_equation(obj[0], obj[7], obj[8]); // plane0
    result[2] = plane_equation(obj[1], obj[0], obj[8]); // plane1
    result[3] = plane_equation(obj[2], obj[1], obj[8]); // plane2
    result[4] = plane_equation(obj[3], obj[2], obj[8]); // plane3
    result[5] = plane_equation(obj[4], obj[3], obj[8]); // plane4
    result[6] = plane_equation(obj[5], obj[4], obj[8]); // plane5
    result[7] = plane_equation(obj[6], obj[5], obj[8]); // plane6
    result[8] = plane_equation(obj[7], obj[6], obj[8]); // plane7
    return result;
}

std::vector <V4> visibility(std::vector <V4> list)
{
    V4 vec = {0, 0, -600, 1};
    std::vector <V4> result;
    for(size_t i = 0; i < list.size(); ++i)
    {
        switch(i)
        {
            case 0:
                printf("#UP IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i])); 
                if(scalar_mult(vec, list[i]) > 0)
                {
                    result.push_back(list[i]);
                }
                else
                {}
                break;

            case 1:
                printf("#BACK IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i])); 
                if(scalar_mult(vec, list[i]) > 0)
                {
                    result.push_back(list[i]);
                }
                else
                {}
                break;

            case 2:
                printf("#RIGTH IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i])); 
                if(scalar_mult(vec, list[i]) > 0)
                {
                    result.push_back(list[i]);
                }
                else
                {}
                break;

            case 3:
                printf("#BOTTOM IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i])); 
                if(scalar_mult(vec, list[i]) > 0)
                {
                    result.push_back(list[i]);
                }
                else
                {}
                break;

            case 4:
                printf("#LEFT IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i])); 
                if(scalar_mult(vec, list[i]) > 0)
                {
                    result.push_back(list[i]);
                }
                else
                {}
                break;

            case 5:
                printf("#FRONT IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i])); 
                if(scalar_mult(vec, list[i]) > 0)
                {
                    result.push_back(list[i]);
                }
                else
                {}
                break;

            case 6:
                printf("#FRONT IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i])); 
                if(scalar_mult(vec, list[i]) > 0)
                {
                    result.push_back(list[i]);
                }
                else
                {}
                break;
        
            case 7:
                printf("#FRONT IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i])); 
                if(scalar_mult(vec, list[i]) > 0)
                {
                    result.push_back(list[i]);
                }
                else
                {}
                break;

            case 8:
                printf("#FRONT IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i])); 
                if(scalar_mult(vec, list[i]) > 0)
                {
                    result.push_back(list[i]);
                }
                else
                {}
                break;
        }
    }
    return result;
}


std::vector <Edge> edges_to_render(std::vector <V4> planes, std::vector <std::vector <int>> connections, std::vector <Point> obj) 
{
    std::vector <Edge> result;
    for(size_t i = 0; i < planes.size(); ++i)
    {
        for(size_t j = 0; j < obj.size(); ++j)
        {
            // if point belongs to plane ...
            V4 v = {obj[j].x, obj[j].y, obj[j].z, 1};
            if(std::abs(std::round(scalar_mult(v, planes[i])*1000)/1000) == 0.000)
            {
                for(size_t t = 0; t < connections[j].size(); ++t)
                {
                    V4 v1 = {obj[connections[j][t]].x, obj[connections[j][t]].y, obj[connections[j][t]].z, 1};
                    if(std::abs(std::round(scalar_mult(v1, planes[i])*1000)/1000) == 0.000)
                    {
                        Edge edges = {obj[j], obj[connections[j][t]]};
                        result.push_back(edges);
                        printf("\tEDGE TO RENDER: %ld, %d\n", j, connections[j][t]);
                    }
                }
            }
        }
    }
    return result;
}


void transform(std::vector <Point> &obj, double k)
{
    for(size_t i = 0; i < obj.size(); ++i)
    {
        obj[i] = {obj[i].x * k, obj[i].y * k, obj[i].z * k};
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
Point central_projection(Point a, double k)
{
    Point result;
    result.y = (k * a.y) / (a.z + k);
    result.x = (k * a.x) / (a.z + k);
    return result;
}

//perspective projection for an object
void central_projection(std::vector <Point> &obj, Point origin, double k)
{
    for(size_t i = 0; i < obj.size(); ++i)
    {
        obj[i] = real_point(origin, central_projection(obj[i], k));
    }
}

//perspective projection for array of edges
void central_projection(std::vector <Edge> &edges, Point origin, double k)
{
    for(size_t i = 0; i < edges.size(); ++i)
    {
        edges[i] = {real_point(origin, central_projection(edges[i].a, k)),
                    real_point(origin, central_projection(edges[i].b, k))};
    }
}

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

void rotate(Axis axis, std::vector <Point> &obj, double mult, int dir, double k)
{
    for(size_t i = 0; i < obj.size(); ++i)
    {
        switch(axis)
        {
            case X:
                obj[i] = rotate_x(obj[i], mult, dir); 
                if(mult != 0)
                {
                    printf("A%ld = {%f, %f, %f}\n", i, obj[i].x, obj[i].y, obj[i].z);
                }
                break;

            case Y:
                obj[i] = rotate_y(obj[i], mult, dir); 
                if(mult != 0)
                {
                    printf("A%ld = {%f, %f, %f}\n", i, obj[i].x, obj[i].y, obj[i].z);
                }
                break;

            case Z:
                obj[i] = rotate_z(obj[i], mult, dir); 
                if(mult != 0)
                {
                    printf("A%ld = {%f, %f, %f}\n", i, obj[i].x, obj[i].y, obj[i].z);
                }
                break;
            default:
                break;
        }
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
    double r = a / 2 * 1.4 + a / 2;
    ///

    /// points for pyramid 
    std::vector <Point> pyramid = {{a / 2, -h / 2, -r}, {r, -h / 2, -a / 2}, {r, -h / 2, a / 2}, {a / 2, -h / 2, r},
                    {-a / 2, -h / 2, r}, {-r, -h / 2, a / 2}, {-r, -h / 2, -a / 2}, {-a / 2, -h / 2, -r}, {0, h / 2, 0}};

    /// points for cube
    std::vector <Point> cube = {{100, 100, -100}, {100, 100, 100}, {-100, 100, 100}, {-100, 100, -100},
                    {100, -100, -100}, {100, -100, 100}, {-100, -100, 100}, {-100, -100, -100}};
    
    std::vector <std::vector <int>> connections_cube = 
    {{1, 3, 4},
     {0, 2, 5},
     {1, 3, 6},
     {2, 0, 7},
     {0, 5, 7},
     {4, 1, 6},
     {5, 2, 7},
     {6, 3, 4}
    };

    std::vector <std::vector <int>> connections_pyr = 
    {{1, 7, 8},
     {0, 2, 8},
     {1, 3, 8},
     {2, 4, 8},
     {3, 5, 8},
     {4, 6, 8},
     {5, 7, 8},
     {6, 0, 8},
     {0, 1, 2, 3, 4, 5, 6, 7}
    };

    //##############
    std::vector <Edge> edges;
    //##############
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
                rotate(axis, pyramid, mult, dir, k);

                // visibility() returns all visible planes of an object
                edges = edges_to_render(visibility(pyramid_planeset(pyramid)), connections_pyr, pyramid);
                central_projection(edges, origin, k);
                
                //////
                draw_obj(renderer, edges, color);
                mult = 0;
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
                                if(scale_time + 0.1 <= 3)
                                {
                                    scale += 0.1;
                                    scale_time += 0.1;
                                    transform(pyramid, scale);
                                    printf("SCALE = %f, scale_time = %f\n", scale, scale_time);
                                    scale = 1;
                                }
                            }
                            else if(event.wheel.y < 0) // scroll down
                            {
                                if(scale_time - 0.1 >= 0.1)
                                {
                                    scale -= 0.1;
                                    scale_time -= 0.1;
                                    transform(pyramid, scale);
                                    printf("SCALE = %f, scale_time = %f\n", scale, scale_time);
                                    scale = 1;
                                }
                                else
                                {
                                    printf("SCALE arg is < 0.1!!!\n");
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
