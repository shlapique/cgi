#include "Math.h"

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

Point real_point(Point origin, Point a)
{   
    Point point = {origin.x + a.x, origin.y - a.y, origin.z + a.z};
    return point;
}

//screen center
Point find_origin(int size_x, int size_y, double k)
{
    Point point = {std::round(size_x / 2), std::round(size_y / 2), k};
    return point;
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
                        //printf("\tEDGE TO RENDER: %ld, %d\n", j, connections[j][t]);
                    }
                }
            }
        }
    }
    return result;
}

std::vector <V4> visibility(std::vector <V4> list)
{
    V4 vec = {0, 0, -600, 1};
    std::vector <V4> result;
    for(size_t i = 0; i < list.size(); ++i)
    {
        if(scalar_mult(vec, list[i]) > 0)
        {
            result.push_back(list[i]);
        }
        switch(i)
        {
            case 0:
                printf("#UP IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i]));
                break;

            case 1:
                printf("#BACK IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i]));
                break;

            case 2:
                printf("#RIGTH IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i]));
                break;

            case 3:
                printf("#BOTTOM IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i]));
                break;

            case 4:
                printf("#LEFT IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i]));
                break;

            case 5:
                printf("#FRONT IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i]));
                break;

            case 6:
                printf("#FRONT IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i]));
                break;

            case 7:
                printf("#FRONT IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i]));
                break;

            case 8:
                printf("#FRONT IS VISIBLE = %ld :: scalar_mult = %f\n", i, scalar_mult(vec, list[i]));
                break;
        }
    }
    return result;
}
