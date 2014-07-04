#pragma once
#include <map>
#include <memory>

namespace GameLayer
{
  class Texture
  {
  public:
    virtual ~Texture(){}

    virtual float GetWidth() = 0;
    virtual float GetHeight() = 0;
    virtual void SetImage(char* a_path) = 0;

    static unsigned int Create(char* a_path, std::string a_name);
    static unsigned int GetMissing();
    static std::weak_ptr<Texture> Get(unsigned int a_id);
    static void SetMissing(char* a_path);
    static std::map <unsigned int, std::shared_ptr<Texture>> TextureDatabase;
    static std::map <std::string, unsigned int> IdMap;
  };
};
