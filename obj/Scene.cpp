#include "Scene.h"
#include "Math.h"
#include "Core.h"

Scene::Scene()
{
    printf("/// EMPTY SCENE CREATED! ///\n");
}

// Where dir = 1 or -1
Point Scene::rotate_x(Point a, double phi, int dir)
{
    Point result;
    result.x = a.x;
    result.y = a.y * std::cos(phi) - dir * a.z * std::sin(phi);
    result.z = dir * a.y * std::sin(phi) + a.z * std::cos(phi);
    return result;
}

Point Scene::rotate_y(Point a, double phi, int dir)
{
    Point result;
    result.x = a.x * std::cos(phi) + dir * a.z * std::sin(phi);
    result.y = a.y;
    result.z = -dir * a.x * std::sin(phi) + a.z * std::cos(phi);
    return result;
}

Point Scene::rotate_z(Point a, double phi, int dir)
{
    Point result;
    result.x = a.x * std::cos(phi) - dir * a.y * std::sin(phi);
    result.y = dir * a.x * std::sin(phi) + a.y * std::cos(phi);
    result.z = a.z;
    return result;
}

//perspective projection
Point Scene::central_projection(Point a, double k)
{
    Point result;
    result.y = (k * a.y) / (a.z + k);
    result.x = (k * a.x) / (a.z + k);
    return result;
}

//perspective projection for an object
void Scene::central_projection(std::vector <Point> &obj, Point origin, double k)
{
    for(size_t i = 0; i < obj.size(); ++i)
    {
        obj[i] = real_point(origin, central_projection(obj[i], k));
    }
}

//perspective projection for array of edges
void Scene::central_projection(std::vector <Edge> &edges, Point origin, double k)
{
    for(size_t i = 0; i < edges.size(); ++i)
    {
        edges[i] = {real_point(origin, central_projection(edges[i].a, k)),
                    real_point(origin, central_projection(edges[i].b, k))};
    }
}

void Scene::isometric_projection(std::vector <Point> &obj, Point origin)
{
    for(size_t i = 0; i < obj.size(); ++i)
    {
        obj[i] = real_point(origin, obj[i]);
    }
}

//isomtric projection for array of edges
void Scene::isometric_projection(std::vector <Edge> &edges, Point origin)
{
    for(size_t i = 0; i < edges.size(); ++i)
    {
        edges[i] = {real_point(origin, edges[i].a), real_point(origin, edges[i].b)};
    }
}

void Scene::rotate(Axis axis, std::vector <Point> &obj, double mult, int dir, double k)
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

std::vector <V4> Scene::visibility(std::vector <V4> list)
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
