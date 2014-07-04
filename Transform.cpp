#include "Transform.h"
extern "C"
{
  #include "glew.h"
}
#include "wglext.h"

using namespace GameLayer;

class Transform_OpenGL : public Transform
{
public:
  Transform_OpenGL() :
    m_position(), m_rotation(0), m_scale(1)
  {
  }


  ~Transform_OpenGL()
  {
  }

  virtual void SetPosition(Vector2f a_position)
  {
    m_position = a_position;
  }
  virtual void SetRotation(float a_rotation)
  {
    m_rotation = a_rotation;
  }
  virtual void SetScale(float a_scale)
  {
    m_scale = a_scale;
  }

  virtual const Vector2f GetPosition() const
  {
    return m_position;
  }
  virtual const float GetRotation() const
  {
    return m_rotation;
  }
  virtual const float GetScale() const
  {
    return m_scale;
  }

  virtual void Translate(Vector2f a_amount)
  {
    m_position += a_amount;
  }
  virtual void Rotate(float a_amount)
  {
    m_rotation += a_amount;
  }
  virtual void Scale(float a_amount)
  {
    m_scale *= a_amount;
  }

  virtual void Apply()
  {
    glPushMatrix();
    glTranslatef(m_position.x, m_position.y, 0);
    glScalef(m_scale, m_scale, m_scale);
    glRotatef(m_rotation, 0, 0, 1);
  }
  virtual void Discard()
  {
    glPopMatrix();
  }
private:
  Vector2f m_position;
  float m_rotation;
  float m_scale;
};


std::shared_ptr<Transform> Transform::Create()
{
  return std::shared_ptr<Transform>(new Transform_OpenGL());
}