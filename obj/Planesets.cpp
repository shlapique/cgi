#include "Planesets.h"

std::vector <V4> planeset_pyr(std::vector <Point> vertex)
{
    std::vector <V4> result(9);
    result[0] = plane_equation(vertex[0], vertex[1], vertex[2]); // bottom
    result[1] = plane_equation(vertex[0], vertex[7], vertex[8]); // plane0
    result[2] = plane_equation(vertex[1], vertex[0], vertex[8]); // plane1
    result[3] = plane_equation(vertex[2], vertex[1], vertex[8]); // plane2
    result[4] = plane_equation(vertex[3], vertex[2], vertex[8]); // plane3
    result[5] = plane_equation(vertex[4], vertex[3], vertex[8]); // plane4
    result[6] = plane_equation(vertex[5], vertex[4], vertex[8]); // plane5
    result[7] = plane_equation(vertex[6], vertex[5], vertex[8]); // plane6
    result[8] = plane_equation(vertex[7], vertex[6], vertex[8]); // plane7
    return result;
}

std::vector <V4> planeset_cube(std::vector <Point> vertex)
{
    std::vector <V4> result(6);
    result[0] = plane_equation(vertex[0], vertex[3], vertex[2]); // up
    result[1] = plane_equation(vertex[1], vertex[2], vertex[5]); // back
    result[2] = plane_equation(vertex[1], vertex[5], vertex[4]); // right
    result[3] = plane_equation(vertex[4], vertex[5], vertex[6]); // bottom
    result[4] = plane_equation(vertex[6], vertex[2], vertex[3]); // left
    result[5] = plane_equation(vertex[0], vertex[4], vertex[7]); // front
    return result;
}

std::vector <V4> planeset_tetra(std::vector <Point> vertex)
{
    std::vector <V4> result(4);
    result[0] = plane_equation(vertex[0], vertex[1], vertex[2]); // bottom
    result[1] = plane_equation(vertex[0], vertex[2], vertex[3]); // front 
    result[2] = plane_equation(vertex[0], vertex[3], vertex[1]); // right
    result[3] = plane_equation(vertex[1], vertex[3], vertex[2]); // bottom
    return result;
}
