#include "Mesh.h"
#include "TriangulatedObject.h"

using namespace GameLayer;

Mesh::Mesh(Vertex* a_vertices, int a_num, std::vector<Vertex>* a_allVerts)
: hull(nullptr), allVerts(a_allVerts)
{
  for (int i = 0; i < a_num; i++)
  {
    vertices.push_back(&a_vertices[i]);
  }
  Triangulate();

  CreateHull();
}


Mesh::~Mesh()
{
}

void Mesh::CreateHull()
{
  hull.reset(new Hull(vertices));
}

Vector2f Mesh::GetCircumCenter(Vertex& vfirst, Vertex& vsecond, Vertex& vthird)
{
  const Vector2f first(vfirst.m_position);
  const Vector2f second(vsecond.m_position);
  const Vector2f third(vthird.m_position);

  Vector2f U;

  float D = 2 * (first.x * (second.y - third.y) +
    second.x * (third.y - first.y) +
    third.x * (first.y - second.y));

  U.x =
    (first.MagnitudeSquared() * (second.y - third.y) +
    second.MagnitudeSquared() * (third.y - first.y) +
    third.MagnitudeSquared() * (first.y - second.y)) / D;
  U.y =
    (first.MagnitudeSquared() * (third.x - second.x) +
    second.MagnitudeSquared() * (first.x - third.x) +
    third.MagnitudeSquared() * (second.x - first.x)) / D;

  return U;
}

bool Mesh::EvaluateTriangle(Vertex& first, Vertex& second, Vertex& third)
{
  // Find circumcircle
  Vector2f onetotwo = second.m_position - first.m_position;
  Vector2f onetothree = third.m_position - first.m_position;

  onetotwo.Normalize();
  onetothree.Normalize();

  // If practically colinear, stop.
  if (Vector2f::ccw(first.m_position, second.m_position, third.m_position) == 0)
  {
    return false;
  }

  Vector2f cc = GetCircumCenter(first, second, third);
  float ccRadius = (cc - first.m_position).Magnitude();

  int s = vertices.size();
  for (int point = 0; point < s; point++)
  {
    if (vertices[point] == &first || vertices[point] == &second || vertices[point] == &third)
    {
      continue;
    }

    if (vertices[point]->m_position.Distance(cc) < ccRadius)
      return false;
  }
  return true;
}

void Mesh::Triangulate()
{
  int end = vertices.size();
  for (int first = 0; first < end - 2; first++)
  {
    for (int second = first + 1; second < end - 1; second++)
    {
      for (int third = second + 1; third < end; third++)
      {
        if (EvaluateTriangle(*vertices[first], *vertices[second], *vertices[third]))
        {
          auto t = std::shared_ptr<Triangle>(new Triangle(vertices[first], vertices[second], vertices[third]));
          triangles.push_back(std::shared_ptr<Triangle>(t));

          LinkTriangleToEdges(vertices[first], vertices[second], vertices[third], t);
        }
      }
    }
  }
}

void Mesh::Render()
{
  for (auto i : triangles)
  {
    i->Render();
  }
  if(Triangle::g_DrawLinesAndPoints)
    hull->Render();
}

std::shared_ptr<Edge> Mesh::FindEdge(Vertex* a, Vertex* b)
{
  Edge temp(a, b);
  for (auto i : edges)
  {
    if (i->Equals(&temp))
    {
      return i;
    }
  }

  return nullptr;
}

void Mesh::LinkTriangleToEdges(Vertex* a, Vertex* b, Vertex* c, std::shared_ptr<Triangle> triangle)
{
  std::shared_ptr<Edge> edge1 = FindEdge(a, b);
  std::shared_ptr<Edge> edge2 = FindEdge(a, c);
  std::shared_ptr<Edge> edge3 = FindEdge(c, b);

  if (edge1 == nullptr)
  {
    edge1.reset(new Edge(a, b));
    edges.push_back(std::shared_ptr<Edge>(edge1));
  }
  if (edge2 == nullptr)
  {
    edge2.reset(new Edge(a, c));
    edges.push_back(std::shared_ptr<Edge>(edge2));
  }
  if (edge3 == nullptr)
  {
    edge3.reset(new Edge(c, b));
    edges.push_back(std::shared_ptr<Edge>(edge3));
  }

  edge1->RegisterTriangle(triangle);
  edge2->RegisterTriangle(triangle);
  edge3->RegisterTriangle(triangle);
}

void Mesh::UnlinkTriangle(std::shared_ptr<Triangle> triangle)
{
  std::shared_ptr<Edge> edge1 = FindEdge(triangle->m_first, triangle->m_second);
  std::shared_ptr<Edge> edge2 = FindEdge(triangle->m_first, triangle->m_third);
  std::shared_ptr<Edge> edge3 = FindEdge(triangle->m_third, triangle->m_second);

  edge1->UnregisterTriangle(triangle.get());
  edge2->UnregisterTriangle(triangle.get());
  edge3->UnregisterTriangle(triangle.get());

  for (auto i = triangles.begin(); i != triangles.end(); i++)
  {
    if (i->get() == triangle.get())
    {
      triangles.erase(i);
      break;
    }
  }
}

void Mesh::Merge(Mesh* other, TriangulatedObject* obj)
{
  for (auto i : other->triangles)
  {
    triangles.push_back(i);
  }
  other->triangles.clear();
  for (auto i : other->edges)
  {
    edges.push_back(i);
  }
  other->edges.clear();
  for (auto i : other->vertices)
  {
    vertices.push_back(i);
  }
  other->vertices.clear();
  obj->RemoveMesh(other);
}