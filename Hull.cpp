#include "Hull.h"
#include "Vector2.h"
#include <vector>
extern "C" {
#include "glew.h" 
}
#include "gl.h"
#include <algorithm>
#include "Core.h"
#include <iostream>

using namespace GameLayer;

Hull::~Hull()
{
}

Vertex* pivot;

// Took Graham Scan code from: http://pastebin.com/5c9PHBcp
inline int distsqr(const Vector2f &a, const Vector2f &b){
  return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}

inline int cross(const Vector2f &a, const Vector2f &b, const Vector2f &c){
  return (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y);
}

// To check ccw-ness.
bool angleCmp(Vertex* self, Vertex* that){
  int t = cross(pivot->m_position, that->m_position, self->m_position);
  if (t < 0) return true;
  if (t == 0){ // Collinear
    return (distsqr(pivot->m_position, self->m_position) <= distsqr(pivot->m_position, that->m_position));
  }
  return false;
}

void graham(std::vector<Vertex*>& p, std::vector<Vertex*>& convexHull)
{
  // Find lowest Y point
  for (int i = 1; i<p.size(); ++i){
    if (p[i]->m_position.y < p[0]->m_position.y || (p[i]->m_position.y == p[0]->m_position.y && abs(p[i]->m_position.x - Core::ScreenWidth/2) < abs(p[0]->m_position.x - Core::ScreenWidth/2)))
      std::swap(p[0], p[i]);
  }

  // Set pivot to lowest Y point and sort depending on angles. 
  pivot = p[0];
  std::sort(p.begin(), p.end(), angleCmp);

  std::vector<Vertex*> chull(p.begin(), p.begin() + 3);

  for (int i = 3; i<p.size(); ++i)
  {
    while (chull.size() >= 2 && 
      Vector2f::ccw(chull[chull.size() - 2]->m_position, chull[chull.size() - 1]->m_position, p[i]->m_position) != -1)
    {
      chull.erase(chull.end() - 1);
    }
    chull.push_back(p[i]);
  }
  convexHull = chull;
}


void Hull::ConstructFromPointSet(std::vector<Vertex*>& vertices)
{
  graham(vertices, hullPointList);

  topVertex = hullPointList[0];
  for (auto i : hullPointList)
  {
    if (i->m_position.y > topVertex->m_position.y || 
      (i->m_position.y == topVertex->m_position.y && 
      abs(i->m_position.x - Core::ScreenWidth / 2) < abs(topVertex->m_position.x - Core::ScreenWidth / 2)))
      topVertex = i;
  }
  bottomVertex = hullPointList[0];
  hullChain.reset(new HullNode(bottomVertex, nullptr, nullptr));
  std::shared_ptr<HullNode> currentNode = hullChain;
  for (int i = 1; i < hullPointList.size(); i++)
  {
    currentNode->next.reset(new HullNode(hullPointList[i], currentNode, nullptr));
    currentNode = currentNode->next;
  }
}

void Hull::Render()
{
  glLineWidth(2);
  glBegin(GL_LINES);
  glColor3f(1, 0, 1);

  int s = hullPointList.size();

  glVertex2f(hullPointList[0]->m_position.x, hullPointList[0]->m_position.y);
  glVertex2f(hullPointList[s-1]->m_position.x, hullPointList[s-1]->m_position.y);

  for (int i = 1; i < s; i++)
  {
    glVertex2f(hullPointList[i]->m_position.x, hullPointList[i]->m_position.y);
    glVertex2f(hullPointList[i-1]->m_position.x, hullPointList[i-1]->m_position.y);
  }
  glEnd();
  glPointSize(10);
  glBegin(GL_POINTS);
  glColor3f(1, 0.5, 1);


  for (int i = 1; i < s; i++)
  {
    glVertex2f(hullPointList[i]->m_position.x, hullPointList[i]->m_position.y);
  }
  glEnd();

  glPointSize(20);
  glBegin(GL_POINTS);
  glColor3f(1, 0, 1);
  glVertex2f(bottomVertex->m_position.x, bottomVertex->m_position.y);
  glVertex2f(topVertex->m_position.x, topVertex->m_position.y);
  glEnd();
}