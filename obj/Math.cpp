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
