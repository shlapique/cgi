#include "Object.h"

Object::Object()
{
    printf("++++EMPTY OBJ CREATED!++++\n");
}

void Object::create_pyramid(double a, double h)
{
    printf("_____PYRAMID's CREATED______\n");
    double r = a / 2 * 1.4 + a / 2;
    this->vertex = {{a / 2, -h / 2, -r}, {r, -h / 2, -a / 2}, {r, -h / 2, a / 2}, {a / 2, -h / 2, r},
                    {-a / 2, -h / 2, r}, {-r, -h / 2, a / 2}, {-r, -h / 2, -a / 2}, {-a / 2, -h / 2, -r}, {0, h / 2, 0}};
    this->planeset = 
    {{0, 1, 2}, // bottom
     {0, 7, 8}, // plane0
     {1, 0, 8}, // .. 
     {2, 1, 8},
     {3, 2, 8}, // .. 
     {4, 3, 8},
     {5, 4, 8},
     {6, 5, 8}, // .. 
     {7, 6, 8}};// plane7

    this->connections = 
    {{1, 7, 8},
     {0, 2, 8},
     {1, 3, 8},
     {2, 4, 8},
     {3, 5, 8},
     {4, 6, 8},
     {5, 7, 8},
     {6, 0, 8},
     {0, 1, 2, 3, 4, 5, 6, 7}};
}

void Object::create_cube(double a)
{
    printf("_____CUBE's CREATED______\n");
    this->vertex = {{a / 2, a / 2, -a / 2}, {a / 2, a / 2, a / 2}, {-a / 2, a / 2, a / 2}, {-a / 2, a / 2, -a / 2},
                    {a / 2, -a / 2, -a / 2}, {a / 2, -a / 2, a / 2}, {-a / 2, -a / 2, a / 2}, {-a / 2, -a / 2, -a / 2}};
    this->planeset = 
    {{0, 3, 2}, // up
     {1, 2, 5}, // back
     {1, 5, 4}, // right
     {4, 5, 6}, // bottom
     {6, 2, 3}, // left
     {0, 4, 7}};// front

    this->connections = 
    {{1, 3, 4},
     {0, 2, 5},
     {1, 3, 6},
     {2, 0, 7},
     {0, 5, 7},
     {4, 1, 6},
     {5, 2, 7},
     {6, 3, 4}};
}


void Object::create_tetra(double a)
{
    printf("_____TETRA's CREATED______\n");
    this->vertex = {{a / 2, -a / 2, -a / 2}, {0, -a / 2, a / 2}, {-a / 2, -a / 2, -a / 2}, {0, a / 2, 0}};
    this->planeset = 
    {{0, 1, 2}, // bottom
     {0, 2, 3}, // front
     {0, 3, 1}, // right
     {1, 3, 2}};// left

    this->connections = 
    {{1, 2, 3},
     {0, 2, 3},
     {1, 3, 0},
     {0, 1, 2}};
}

// a -- for bigger radius of bottom ellipse, b -- for smaller one.
void Object::create_trunc_cylinder(double a, double b, double prec)
{
    printf("_____TRUNC_CYLINGER's CREATED______\n");
    this->vertex = 
    {{0, 0, -b / 2}, {a / 2, 0, 0}, {0, 0, b / 2}, {-a / 2, 0, 0}};
    this->planeset = 
    {{0, 3, 2},
     {0, 1, 2}};

    this->connections = 
    {{1, 3},
     {0, 2},
     {1, 3},
     {2, 0}};
}
