#pragma once
#include "Object.h"

namespace GameLayer
{
  class RootNode :
    public Object
  {
  public:
    RootNode();
    virtual ~RootNode();

    virtual void Update(float a_deltaTime);
  };
};
