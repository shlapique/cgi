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

void Scene::tri_central_projection(Point origin, double k)
{
    for(size_t i = 0; i < tri.size(); ++i)
    {
        this->tri[i] = {real_point(origin, point_central_projection(tri[i][0], k)),
                    real_point(origin, point_central_projection(tri[i][1], k)), real_point(origin, point_central_projection(tri[i][2], k))};
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
    this->edges = edges_to_render(visibility(get_planeset(vertex, planeset)), connections, vertex, tri);
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


void Scene::draw_obj(SDL_Renderer *renderer, std::vector <Edge> edges, std::vector <std::vector <Point>> tri, Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    unsigned char tri_color_r = 200;
    unsigned char tri_color_g = 123;
    unsigned char tri_color_b = 11;
    // 0 stands for a, 1 stands for b
    bool flag = 0;
    for(size_t i = 0; i < edges.size(); ++i)
    {
        draw_segment(renderer, edges[i].a, edges[i].b, color);
        ///
        ///
    }

 //   printf("TRI SIZE IS %ld = tri.size()\n\n", tri.size());
    for(size_t j = 0; j < tri.size(); ++j)
    {
        //printf("\tCOLORED!\n");
        std::vector <SDL_Vertex> verts =
        {{SDL_FPoint{(float)tri[j][0].x, (float)tri[j][0].y}, SDL_Color{ tri_color_r, 0, 0, 255 }, SDL_FPoint{ 0 }},
            {SDL_FPoint{(float)tri[j][1].x, (float)tri[j][1].y}, SDL_Color{tri_color_g, 0, 0, 255 }, SDL_FPoint{ 0 }},
            {SDL_FPoint{(float)tri[j][2].x, (float)tri[j][2].y}, SDL_Color{tri_color_b, 0, 0, 255 }, SDL_FPoint{ 0 }}};
        SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
        //printf("\tCOLORED!\n");
    }
}

void Scene::central_projection(Point origin, double k)
{
    this->edges = edges_to_render(visibility(get_planeset(vertex, planeset)), connections, vertex, tri);
    edges_central_projection(origin, k);
    tri_central_projection(origin, k);
}

void Scene::draw(Color color)
{
    draw_obj(renderer, edges, tri, color);
}
