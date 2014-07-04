#include "Edge.h"
#include "Triangle.h"

using namespace GameLayer;

Edge::~Edge()
{
}

void Edge::RegisterTriangle(std::weak_ptr<Triangle> triangle)
{
  bool exists = false;
  for (auto i = parentTriangles.begin(); i != parentTriangles.end();i++)
  {
    if (i->lock().get() == triangle.lock().get())
    {
      exists = true;
      break;
    }
  }
  if (!exists)
    parentTriangles.push_back(triangle);
}
void Edge::UnregisterTriangle(Triangle* triangle)
{
  for (auto i = parentTriangles.begin(); i != parentTriangles.end();)
  {
    if (i->lock().get() == triangle || i->lock().get() == nullptr)
      i = parentTriangles.erase(i);
    else
      i++;
  }
}