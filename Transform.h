#pragma once
#include "Vector2.h"
#include <memory>

namespace GameLayer
{
  class Transform
  {
  public:
    virtual ~Transform(){}

    virtual void SetPosition(Vector2f a_position) = 0;
    virtual void SetRotation(float a_rotation) = 0;
    virtual void SetScale(float a_scale) = 0;

    virtual const Vector2f GetPosition() const = 0;
    virtual const float GetRotation() const = 0;
    virtual const float GetScale() const = 0;

    virtual void Translate(Vector2f a_amount) = 0;
    virtual void Rotate(float a_amount) = 0;
    virtual void Scale(float a_amount) = 0;

    virtual void Apply() = 0;
    virtual void Discard() = 0;

    static std::shared_ptr<Transform> Create();
  };
};
