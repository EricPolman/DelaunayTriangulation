#pragma once
#include "Object.h"
#include "Triangle.h"
#include "Mesh.h"
#include <vector>

namespace GameLayer
{
  class TriangulatedObject :
    public Object
  {
  public:
    TriangulatedObject(std::weak_ptr<Object> a_parent, 
      std::string a_name, int a_numberOfVertices);
    virtual ~TriangulatedObject();

    virtual void Update(float a_fDeltaTime);
    virtual void Render();
    void RemoveMesh(Mesh* mesh);

    void AddVertex(int x, int y, bool addBias = false);

  private:
    void SortVerticesOnX(int start, int size);
    void SortVerticesOnY(int start, int size);
    std::vector<Vertex> m_vertices;

    std::vector<Mesh> m_meshes;

    bool* m_positionStencil;

    void MergeMeshes(Mesh* left, Mesh* right);

    bool EvaluateTriangle(Vertex& first, Vertex& second, Vertex& third);
  };
};
