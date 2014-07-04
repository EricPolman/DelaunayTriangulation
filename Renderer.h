#pragma once
#include "Vector2.h"
#include <memory>

namespace GameLayer
{
  class Renderer
  {
  public:
    virtual ~Renderer(){}

    virtual void SetDimensions(Vector2f a_dimensions) = 0;
    virtual const Vector2f& GetDimensions() = 0;

    virtual unsigned int GetTexture() = 0;
    virtual void SetTexture(unsigned int a_id) = 0;
    virtual void Render() = 0;

    static std::shared_ptr<Renderer> Create(Vector2f a_dimensions, unsigned int a_textureId);
  };
};
