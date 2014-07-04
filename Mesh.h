#pragma once
#include "Hull.h"
#include <vector>
#include "Triangle.h"
#include "Edge.h"
#include <memory>

namespace GameLayer
{
  class TriangulatedObject;

  class Mesh
  {
  public:
    Mesh(Vertex* a_vertices, int a_num, std::vector<Vertex>* a_allVerts);
    Mesh(Mesh* left, Mesh* right);
    ~Mesh();

    void CreateHull();
    void Merge(Mesh* other, TriangulatedObject* obj);
    void Triangulate();

    void Render();

    std::shared_ptr<Hull> hull;
    std::vector<Vertex*> vertices;
    std::vector<std::shared_ptr<Triangle>> triangles;
    std::vector<std::shared_ptr<Edge>> edges;
    std::shared_ptr<Edge> FindEdge(Vertex* a, Vertex* b);
    void LinkTriangleToEdges(Vertex* a, Vertex* b, Vertex* c, std::shared_ptr<Triangle> triangle);
    void UnlinkTriangle(std::shared_ptr<Triangle> triangle);

    std::vector<Vertex>* allVerts;
  private:
    bool EvaluateTriangle(Vertex& first, Vertex& second, Vertex& third);
    Vector2f GetCircumCenter(Vertex& first, Vertex& second, Vertex& third);
  };
};
