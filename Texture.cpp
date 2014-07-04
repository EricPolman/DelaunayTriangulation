#include "Texture.h"
#include "FreeImage.h"
#include <iostream>

extern "C"
{
  #include "glew.h"
}

using namespace GameLayer;

std::map <unsigned int, std::shared_ptr<Texture>> Texture::TextureDatabase;
std::map <std::string, unsigned int> Texture::IdMap;

class Texture_OpenGL : public Texture
{
public:
  Texture_OpenGL(char* a_sPath)
  {
    glGenTextures(1, &m_textureId);    
    SetImage(a_sPath);
  }
  virtual ~Texture_OpenGL(){ glDeleteTextures(1, &m_textureId); }

  virtual void SetImage(char* a_sPath)
  {
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    fif = FreeImage_GetFileType(a_sPath, 0);
    if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(a_sPath);
    FIBITMAP* tmp = FreeImage_Load(fif, a_sPath);
    FIBITMAP* dib = FreeImage_ConvertTo32Bits(tmp);
    m_width = FreeImage_GetWidth(dib);
    m_height = FreeImage_GetHeight(dib);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA8,
      m_width, m_height, 0, GL_BGRA,
      GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(dib)
      );

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    //Remove buffers
    FreeImage_Unload(tmp);
    FreeImage_Unload(dib);

    std::cout << glGetError() << std::endl;
  }
  virtual float GetWidth()
  {
    return m_width;
  }

  virtual float GetHeight()
  {
    return m_height;
  }

  GLuint m_textureId;
protected:
  int m_width, m_height;
};


unsigned int Texture::Create(char* a_path, std::string a_name)
{
  unsigned int id;
  Texture_OpenGL* obj = new Texture_OpenGL(a_path);
  id = obj->m_textureId;
  TextureDatabase[id] = std::shared_ptr<Texture>(obj);
  IdMap[a_name] = id;
  return id;
}
void Texture::SetMissing(char* a_path)
{
  Texture_OpenGL* obj = new Texture_OpenGL(a_path);
  TextureDatabase[1337] = std::shared_ptr<Texture>(obj);
  IdMap["missing"] = 1337;
}

unsigned int Texture::GetMissing()
{
  return dynamic_cast<Texture_OpenGL*>(TextureDatabase[1337].get())->m_textureId;
}


std::weak_ptr<Texture> Texture::Get(unsigned int a_id)
{
  if (TextureDatabase.find(a_id) != TextureDatabase.end())
    return std::weak_ptr<Texture>(TextureDatabase[a_id]);
  else
    return std::weak_ptr<Texture>(TextureDatabase[1337]);
}