#ifndef __FIGURE_H__
#define __FIGURE_H__

#include "Core.h"
#include <vector>

// pyramid for my variant (8 vertecies on bottom)
class Pyramid
{
    public:
        Pyramid(double A, double H, double R)
            : a(A), h(H), r(R) {}
            
        /// points for pyramid 
        std::vector <Point> vertex = {{a / 2, -h / 2, -r}, {r, -h / 2, -a / 2}, {r, -h / 2, a / 2}, {a / 2, -h / 2, r}, 
                            {-a / 2, -h / 2, r}, {-r, -h / 2, a / 2}, {-r, -h / 2, -a / 2}, {-a / 2, -h / 2, -r}, {0, h / 2, 0}};

        std::vector <std::vector <int>> connections = 
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

    private:
        double a;
        double h;
        double r = a / 2 * 1.4 + a / 2;
};

// cube for test
class Cube
{
    public:
        Cube(double a);

        /// points for cube
        std::vector <Point> vertex = {{a, a, -a}, {a, a, a}, {-a, a, a}, {-a, a, -a},
                        {a, -a, -a}, {a, -a, a}, {-a, -a, a}, {-a, -a, -a}};
            
        std::vector <std::vector <int>> connections = 
        {{1, 3, 4}, 
         {0, 2, 5}, 
         {1, 3, 6}, 
         {2, 0, 7}, 
         {0, 5, 7}, 
         {4, 1, 6}, 
         {5, 2, 7}, 
         {6, 3, 4}
        };  

    private:
        double a;
};

#endif
