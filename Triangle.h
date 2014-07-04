#pragma once
#include "Vector2.h"

namespace GameLayer
{
  class Triangle
  {
  public:
    Triangle(Vertex* a_first, Vertex* a_second, Vertex* a_third);
    ~Triangle();

    void Render();
    Vertex* m_first, *m_second, *m_third;

    float r, g, b;

    static bool g_DrawLinesAndPoints;
  };
};
