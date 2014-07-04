#include "Triangle.h"
#include "TriangulatedObject.h"
#include "InputManager.h"

extern "C"
{
#include "glew.h" 
}
#include "gl.h"

using namespace GameLayer;

bool Triangle::g_DrawLinesAndPoints = false;

Triangle::Triangle(Vertex* a_first, Vertex* a_second, Vertex* a_third)
: m_first(a_first), m_second(a_second), m_third(a_third)
{
  Vector2f centroid;
  centroid.x = a_first->m_position.x + ((2 / 3)*a_second->m_position.x - a_first->m_position.x);
  centroid.y = a_first->m_position.y + ((2 / 3)*a_second->m_position.y - a_first->m_position.y);

  float distToFirst = centroid.Distance(a_first->m_position);
  float distToSecond = centroid.Distance(a_second->m_position);
  float distToThird = centroid.Distance(a_third->m_position);
  float totalDist = distToFirst + distToSecond + distToThird;

  r = (m_first->r * distToFirst + m_second->r * distToSecond + m_third->r * distToThird) / totalDist;
  g = (m_first->g * distToFirst + m_second->g * distToSecond + m_third->g * distToThird) / totalDist;
  b = (m_first->b * distToFirst + m_second->b * distToSecond + m_third->b * distToThird) / totalDist;
}

Triangle::~Triangle()
{
}

void Triangle::Render()
{
  glDisable(GL_CULL_FACE);

  glBegin(GL_TRIANGLES);
  glColor3f(r,g,b);
  glVertex2f(m_first->m_position.x, m_first->m_position.y);
  glVertex2f(m_second->m_position.x, m_second->m_position.y);
  glVertex2f(m_third->m_position.x, m_third->m_position.y);
  glEnd();

  glLineWidth(1);
  glBegin(GL_LINES);
  glColor3f(1, 1, 1);
  glVertex2f(m_first->m_position.x, m_first->m_position.y);
  glVertex2f(m_second->m_position.x, m_second->m_position.y);

  glVertex2f(m_first->m_position.x, m_first->m_position.y);
  glVertex2f(m_third->m_position.x, m_third->m_position.y);

  glVertex2f(m_third->m_position.x, m_third->m_position.y);
  glVertex2f(m_second->m_position.x, m_second->m_position.y);
  glEnd();

  if (g_DrawLinesAndPoints)
  {

    glPointSize(6);

    glBegin(GL_POINTS);
    glColor3f(1, 1, 1);
    glVertex2f(m_first->m_position.x, m_first->m_position.y);
    glVertex2f(m_second->m_position.x, m_second->m_position.y);
    glVertex2f(m_third->m_position.x, m_third->m_position.y);
    glEnd();
  }
}