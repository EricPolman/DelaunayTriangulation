#pragma once
#include "Vector2.h"
#include <vector>
#include <list>
#include <memory>

namespace GameLayer
{
  struct HullNode
  {
    HullNode(Vertex* a_vert, std::shared_ptr<HullNode> a_prev, std::shared_ptr<HullNode> a_next)
    : vertex(a_vert), prev(a_prev), next(a_next) {}

    Vertex* vertex;
    std::shared_ptr<HullNode> prev, next;
  };

  class Hull
  {
  public:
    Hull(std::vector<Vertex*>& vertices)
    {
      ConstructFromPointSet(vertices);
    }
    ~Hull();

    void ConstructFromPointSet(std::vector<Vertex*>& vertices);
    void Render();

    std::vector<Vertex*> hullPointList;
    std::shared_ptr<HullNode> hullChain;
    Vertex *topVertex, *bottomVertex;

    static int lastCross;
  private:
  };
};
