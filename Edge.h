#pragma once
#include "Vector2.h"
#include <vector>
#include <memory>

namespace GameLayer
{
  class Triangle;
  class Mesh;

  class Edge
  {
  public:
    Edge(Vertex* a_first, Vertex* a_second)
      : first(a_first), second(a_second) {}
    ~Edge();

    Vertex *first, *second;

    void RegisterTriangle(std::weak_ptr<Triangle> triangle);
    void UnregisterTriangle(Triangle* triangle);
    std::vector<std::weak_ptr<Triangle>> parentTriangles;

    bool Equals(Edge* other)
    {
      if (first == other->first && second == other->second || 
        second == other->first && first == other->second)
        return true;
      return false;
    }

    bool IsUniqueInMesh(Mesh* a_mesh);
  };
};
