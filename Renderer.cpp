#include "Renderer.h"
#include "Texture.h"

extern "C"
{
#include "glew.h" 
}

using namespace GameLayer;

class Renderer_OpenGL : public Renderer
{
public:
  Renderer_OpenGL(Vector2f a_dimensions, unsigned int a_textureId)
    : m_textureId(a_textureId), m_dimensions(a_dimensions)
  {
    auto tex = Texture::TextureDatabase.find(m_textureId);

    if (tex == Texture::TextureDatabase.end())
      m_textureId = Texture::GetMissing();
  }
  virtual ~Renderer_OpenGL()
  {
  }

  virtual void SetDimensions(Vector2f a_dimensions)
  {
    m_dimensions = a_dimensions;
  }
  virtual const Vector2f& GetDimensions()
  {
    return m_dimensions;
  }

  virtual unsigned int GetTexture()
  {
    auto tex = Texture::TextureDatabase.find(m_textureId);
    if (tex == Texture::TextureDatabase.end())
      return Texture::GetMissing();
    return m_textureId;
  }
  virtual void SetTexture(unsigned int a_id)
  {
    m_textureId = a_id;
  }

  virtual void Render()
  {
    //glColor3f( 1.0f, 1.0f, 1.0f );
    glEnable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, GetTexture());

    glBegin(GL_QUADS);
      glNormal3f(0, 0, 1);
      glTexCoord2f(0.0f, 0.0f);
      glVertex2f(-m_dimensions.x / 2, -m_dimensions.y / 2);
      glTexCoord2f(1.0f, 0.0f);
      glVertex2f(m_dimensions.x / 2, -m_dimensions.y / 2);
      glTexCoord2f(1.0f, 1.0f);
      glVertex2f(m_dimensions.x / 2, m_dimensions.y / 2);
      glTexCoord2f(0.0f, 1.0f);
      glVertex2f(-m_dimensions.x / 2, m_dimensions.y / 2);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
  }

private:
  unsigned int m_textureId;
  Vector2f m_dimensions;
};


std::shared_ptr<Renderer> Renderer::Create
  (Vector2f a_dimensions, unsigned int a_textureId)
{
  return std::shared_ptr<Renderer>(new Renderer_OpenGL(a_dimensions, a_textureId));
}