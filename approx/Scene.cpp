#include "Scene.h"

Scene::Scene()
{
    printf("/// EMPTY SCENE CREATED! ///\n");
}

Scene::Scene(SDL_Renderer *renderer)
{
    this->renderer = renderer;
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
Point Scene::point_central_projection(Point a, double k)
{
    Point result;
    result.y = (k * a.y) / (a.z + k);
    result.x = (k * a.x) / (a.z + k);
    return result;
}

//perspective projection for array of edges
void Scene::edges_central_projection(Point origin, double k)
{
    for(size_t i = 0; i < edges.size(); ++i)
    {
        this->edges[i] = {real_point(origin, point_central_projection(edges[i].a, k)),
                    real_point(origin, point_central_projection(edges[i].b, k))};
    }
}

void Scene::vertex_isometric_projection(Point origin)
{
    for(size_t i = 0; i < vertex.size(); ++i)
    {
        this->vertex[i] = real_point(origin, vertex[i]);
    }
}

//isometric projection for array of edges
void Scene::edges_isometric_projection(Point origin)
{
    for(size_t i = 0; i < edges.size(); ++i)
    {
        this->edges[i] = {real_point(origin, edges[i].a), real_point(origin, edges[i].b)};
    }
}

void Scene::isometric_projection(Point origin)
{
    this->edges = edges_to_render(visibility(get_planeset(vertex, planeset)), connections, vertex);
    edges_isometric_projection(origin);
}

void Scene::transform(double k)
{
    for(size_t i = 0; i < vertex.size(); ++i)
    {
        this->vertex[i] = {vertex[i].x * k, vertex[i].y * k, vertex[i].z * k}; 
    }
}

void Scene::rotate(Axis axis, double mult, int dir, double k)
{
    for(size_t i = 0; i < vertex.size(); ++i)
    {
        switch(axis)
        {
            case X:
                this->vertex[i] = rotate_x(vertex[i], mult, dir);
                if(mult != 0)
                {
                    printf("A%ld = {%f, %f, %f}\n", i, vertex[i].x, vertex[i].y, vertex[i].z);
                }
                break;

            case Y:
                this->vertex[i] = rotate_y(vertex[i], mult, dir);
                if(mult != 0)
                {
                    printf("A%ld = {%f, %f, %f}\n", i, vertex[i].x, vertex[i].y, vertex[i].z);
                }
                break;

            case Z:
                this->vertex[i] = rotate_z(vertex[i], mult, dir);
                if(mult != 0)
                {
                    printf("A%ld = {%f, %f, %f}\n", i, vertex[i].x, vertex[i].y, vertex[i].z);
                }
                break;
            default:
                break;
        }
    }
}

void Scene::draw_segment(SDL_Renderer *renderer, Point a, Point b, Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
}


void Scene::draw_obj(SDL_Renderer *renderer, std::vector <Edge> edges, Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    for(size_t i = 0; i < edges.size(); ++i)
    {
        draw_segment(renderer, edges[i].a, edges[i].b, color);
    }
}

void Scene::central_projection(Point origin, double k)
{
    this->edges = edges_to_render(visibility(get_planeset(vertex, planeset)), connections, vertex);
    edges_central_projection(origin, k);
}

void Scene::draw(Color color)
{
    draw_obj(renderer, edges, color);
}
