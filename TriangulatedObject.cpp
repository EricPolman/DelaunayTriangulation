#include "TriangulatedObject.h"

extern "C" { 
  #include "glew.h" 
}
#include "gl.h"
#include <chrono>
#include <random>
#include <limits>
#include "InputManager.h"
#include "Edge.h"
#include "Core.h"

#define SPLITS 4

using namespace GameLayer;

static std::mt19937 RNG;


// Compare methods for sorting and dividing.
int CompareValueOfX(const void* a, const void* b)
{
  int arg1 = (*reinterpret_cast<const Vertex*>(a)).m_position.x;
  int arg2 = (*reinterpret_cast<const Vertex*>(b)).m_position.x;
  if (arg1 < arg2) return -1;
  if (arg1 > arg2) return 1;
  return 0;
}
int CompareValueOfY(const void* a, const void* b)
{
  int arg1 = (*reinterpret_cast<const Vertex*>(a)).m_position.y;
  int arg2 = (*reinterpret_cast<const Vertex*>(b)).m_position.y;
  if (arg1 < arg2) return -1;
  if (arg1 > arg2) return 1;
  return 0;
}

void TriangulatedObject::SortVerticesOnX(int start, int size)
{
  std::qsort(&m_vertices[start], size, sizeof(Vertex), CompareValueOfX);
}
void TriangulatedObject::SortVerticesOnY(int start, int size)
{
  std::qsort(&m_vertices[start], size, sizeof(Vertex), CompareValueOfY);
}

Vector2f GetCircumCenter(Vertex& vfirst, Vertex& vsecond, Vertex& vthird)
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

bool TriangulatedObject::EvaluateTriangle(Vertex& first, Vertex& second, Vertex& third)
{
  // Find circumcircle
  Vector2f onetotwo = second.m_position - first.m_position;
  Vector2f onetothree = third.m_position - first.m_position;

  onetotwo.Normalize();
  onetothree.Normalize();

  if (Vector2f::ccw(first.m_position, second.m_position, third.m_position) == 0)
  {
    return false;
  }

  Vector2f cc = GetCircumCenter(first, second, third);
  float ccRadius = (cc - first.m_position).Magnitude();

  int s = m_vertices.size();
  for (int point = 0; point < s; point++)
  {
    if (&m_vertices[point] == &first || &m_vertices[point] == &second || &m_vertices[point] == &third)
    {
      continue;
    }

    if (m_vertices[point].m_position.Distance(cc) < ccRadius)
      return false;
  }
  return true;
}

TriangulatedObject::TriangulatedObject(std::weak_ptr<Object> a_parent, 
  std::string a_name, int a_numberOfVertices)
: Object(a_parent, a_name)
{
  //m_vertices.reserve(a_numberOfVertices * 1.5f);

  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  m_positionStencil = new bool[Core::ScreenHeight * Core::ScreenWidth];
  memset(m_positionStencil, 0, Core::ScreenHeight * Core::ScreenWidth);

  //RNG.seed(std::chrono::system_clock::now().time_since_epoch().count());
  //srand(std::chrono::system_clock::now().time_since_epoch().count());
  RNG.seed(42);
  srand(15);

  AddVertex(0, 0);
  AddVertex(Core::ScreenWidth, 0);
  AddVertex(0, Core::ScreenHeight-1);
  AddVertex(Core::ScreenWidth, Core::ScreenHeight-1);

#if SPLITS == 4
  AddVertex(Core::ScreenWidth / 4 + Core::ScreenWidth / 8, 0);
  AddVertex(Core::ScreenWidth / 4 + Core::ScreenWidth / 8, Core::ScreenHeight-1);
  AddVertex(Core::ScreenWidth / 4 * 2 + Core::ScreenWidth / 8, 0); // Triggers breakpoint, mem access?
  AddVertex(Core::ScreenWidth / 4 * 2 + Core::ScreenWidth / 8, Core::ScreenHeight-1); // Triggers breakpoint, mem access?
#endif

  for (int i = 0; i < a_numberOfVertices; i++)
  {
    int x = RNG() % (Core::ScreenWidth - 10) + 5;
    int y = RNG() % (Core::ScreenHeight - 10) + 5;
    while (m_positionStencil[y * Core::ScreenWidth + x])
    {
      x = RNG() % (Core::ScreenWidth - 10) + 5;
      y = RNG() % (Core::ScreenHeight - 10) + 5;
    }
    AddVertex(x, y, true);
  }

  SortVerticesOnX(0, m_vertices.size());

#if SPLITS == 4
  m_meshes.push_back(Mesh(&m_vertices.front(), m_vertices.size() / 4, &m_vertices));
  m_meshes.push_back(Mesh(&(m_vertices.front()) + (m_vertices.size() / 4), m_vertices.size() / 4, &m_vertices));
  m_meshes.push_back(Mesh(&(m_vertices.front()) + (m_vertices.size() / 4) * 2, m_vertices.size() / 4, &m_vertices));
  m_meshes.push_back(Mesh(&(m_vertices.front()) + (m_vertices.size() / 4) * 3, m_vertices.size() / 4, &m_vertices));
  MergeMeshes(&m_meshes[0], &m_meshes[1]);
  MergeMeshes(&m_meshes[1], &m_meshes[2]);
  m_meshes[0].CreateHull();
  m_meshes[1].CreateHull();
  MergeMeshes(&m_meshes[0], &m_meshes[1]);
  m_meshes[0].CreateHull();
#elif SPLITS == 2
  m_meshes.push_back(Mesh(&m_vertices.front(), m_vertices.size() / 2, &m_vertices));
  m_meshes.push_back(Mesh(&(m_vertices.front()) + (m_vertices.size() / 2), m_vertices.size() / 2, &m_vertices));
  MergeMeshes(&m_meshes[0], &m_meshes[1]);
  m_meshes[0].CreateHull();
#elif SPLITS == 1
  m_meshes.push_back(Mesh(&m_vertices.front(), m_vertices.size(), &m_vertices));
#endif

}

TriangulatedObject::~TriangulatedObject()
{

}


void TriangulatedObject::AddVertex(int x, int y, bool addBias)
{
  Vertex v;
  if (addBias)
  {
    float posOffset = (RNG() % 5000) / 10000.0f;
    v.m_position = Vector2f(x - 0.25f + posOffset, y - 0.25f + posOffset);
  }
  else
  {
    v.m_position = Vector2f(x, y);
  }
  m_positionStencil[y * Core::ScreenWidth + x] = true;
  float ran = rand() / (float)RAND_MAX;
  ran += v.m_position.x / (Core::ScreenWidth * 4);
  v.r = ran;
  ran = rand() / (float)RAND_MAX;
  ran += v.m_position.y / (Core::ScreenHeight * 4);
  v.g = ran;
  ran = rand() / (float)RAND_MAX;
  v.b = ran;
  m_vertices.push_back(v);
}


void TriangulatedObject::Update(float a_fDeltaTime)
{
  if (Input->IsKeyPressed(KEY_l))
  {
    Triangle::g_DrawLinesAndPoints = !Triangle::g_DrawLinesAndPoints;
  }
}

void TriangulatedObject::Render()
{
  m_transform->Apply();

  if (Triangle::g_DrawLinesAndPoints)
  {
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(0.5, 0.5, 0.5);
    for (auto i : m_vertices)
    {
      glVertex2f(i.m_position.x, i.m_position.y);
    }
    glEnd();
  }
  for (auto i : m_meshes)
  {
    i.Render();
  }

  for (auto i = m_children.begin(); i != m_children.end(); i++)
  {
    i->get()->Render();
  }

  m_transform->Discard();
}

// http://www.personal.psu.edu/cxc11/AERSP560/DELAUNEY/13_Two_algorithms_Delauney.pdf
void TriangulatedObject::MergeMeshes(Mesh* left, Mesh* right)
{
  Edge upperTangent(left->hull->topVertex, right->hull->topVertex);
  Edge lowerTangent(left->hull->bottomVertex, right->hull->bottomVertex);

  std::shared_ptr<HullNode> currentNodeLeft = left->hull->hullChain->next;
  std::shared_ptr<HullNode> currentNodeRight = right->hull->hullChain->next;
  while (currentNodeRight->next != nullptr)
  {
    currentNodeRight = currentNodeRight->next;
  }

  while (!lowerTangent.Equals(&upperTangent))
  {
    if (lowerTangent.first != currentNodeLeft->vertex && EvaluateTriangle(*lowerTangent.first, *lowerTangent.second, *currentNodeLeft->vertex))
    {
      auto t = std::shared_ptr<Triangle>(new Triangle(lowerTangent.first, lowerTangent.second, currentNodeLeft->vertex));
      left->triangles.push_back(t);
      left->LinkTriangleToEdges(lowerTangent.first, lowerTangent.second, currentNodeLeft->vertex, t);

      lowerTangent.first = currentNodeLeft->vertex;
      if (currentNodeLeft->next != nullptr)// && currentNodeLeft->next->vertex->m_position.y > currentNodeLeft->vertex->m_position.y)
      {
        currentNodeLeft = currentNodeLeft->next;
      }
    }
    else if (lowerTangent.second != currentNodeRight->vertex && EvaluateTriangle(*lowerTangent.first, *lowerTangent.second, *currentNodeRight->vertex))
    {
      auto t = std::shared_ptr<Triangle>(new Triangle(lowerTangent.first, lowerTangent.second, currentNodeRight->vertex));
      left->triangles.push_back(t);
      left->LinkTriangleToEdges(lowerTangent.first, lowerTangent.second, currentNodeRight->vertex, t);

      lowerTangent.second = currentNodeRight->vertex;
      if (currentNodeRight->prev != nullptr)// && currentNodeRight->prev->vertex->m_position.y > currentNodeRight->vertex->m_position.y)
      {
        currentNodeRight = currentNodeRight->prev;
      }
    }
    else
    {
      // No combination of vertices possible for valid Delaunay triangle
      // Check if the left edge or right edge belongs to a non-Delaunay triangle
      // Insert newly freed vertex in a node and put in the chain.
      // Continue.
      auto leftEdge = left->FindEdge(lowerTangent.first, currentNodeLeft->vertex);
      if (leftEdge == nullptr)
        return;
      auto leftTriangle = leftEdge->parentTriangles[0].lock();
      if (EvaluateTriangle(*leftTriangle->m_first, *leftTriangle->m_second, *leftTriangle->m_third))
      {
        // Left triangle is ok, now check righter
        auto rightEdge = right->FindEdge(lowerTangent.second, currentNodeRight->vertex);
        if (rightEdge == nullptr)
          return;
        auto rightTriangle = rightEdge->parentTriangles[0].lock();
        if (EvaluateTriangle(*rightTriangle->m_first, *rightTriangle->m_second, *rightTriangle->m_third))
        {
          // Both triangles for both points are valid. 
          // Either passed the top point with tangent.first or node->vertex OR
          // there is a newly created triangle in the way. Which is bad, mkay
          left->Merge(right, this);
          return;
        }
        else
        {
          // Right triangle is non-Delaunay
          // Find other vertex, create node
          Vertex* remainingVert = rightTriangle->m_first;
          if (remainingVert == rightEdge->first || remainingVert == rightEdge->second)
            remainingVert = rightTriangle->m_second;
          if (remainingVert == rightEdge->first || remainingVert == rightEdge->second)
            remainingVert = rightTriangle->m_third;

          auto newHullNode = std::shared_ptr<HullNode>(new HullNode(remainingVert, currentNodeRight, currentNodeRight->next));
          if (currentNodeRight->next != nullptr)
            currentNodeRight->next->prev = newHullNode;
          currentNodeRight->next = newHullNode;
          currentNodeRight = newHullNode;

          // Unlink and remove triangle
          right->UnlinkTriangle(rightTriangle);
        }
      }
      else
      {
        // Left triangle is non-Delaunay
        // Find other vertex, create node
        // Unlink and remove triangle
        Vertex* remainingVert = leftTriangle->m_first;
        if (remainingVert == leftEdge->first || remainingVert == leftEdge->second)
          remainingVert = leftTriangle->m_second;
        if (remainingVert == leftEdge->first || remainingVert == leftEdge->second)
          remainingVert = leftTriangle->m_third;

        auto newHullNode = std::shared_ptr<HullNode>(new HullNode(remainingVert, currentNodeLeft->prev, currentNodeLeft));
        if (currentNodeLeft->prev != nullptr)
          currentNodeLeft->prev->next = newHullNode;
        currentNodeLeft->prev = newHullNode;
        currentNodeLeft = newHullNode;

        // Unlink and remove triangle
        left->UnlinkTriangle(leftTriangle);
      }
    }
  }

  // Transfer triangles and other data from right to left
  left->Merge(right, this);

  // Remove right
  // Also fix adding of tris.
}

void TriangulatedObject::RemoveMesh(Mesh* mesh)
{
  for (auto i = m_meshes.begin(); i != m_meshes.end(); i++)
  {
    if (&(*i) == mesh)
    {
      m_meshes.erase(i);
      break;
    }
  }
}