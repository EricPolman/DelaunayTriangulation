#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>

inline unsigned __int64 RDTSC()
{
  _asm _emit 0x0F
  _asm _emit 0x31
}

class TimerRDTSC
{
  unsigned __int64  start_cycle;
  unsigned __int64  end_cycle;
public:
  inline void Start() { start_cycle = RDTSC(); }
  inline void Stop() { end_cycle = RDTSC(); }
  unsigned __int64 Interval() { return end_cycle - start_cycle; }
};

namespace GameLayer{
  class Object;

  class Core : public Singleton<Core>
  {
    friend class Singleton<Core>;
  public:
    void Init(void* a_pGraphics);
    bool Update(float a_fDeltaTime);
    
    static void* g_pRenderTarget;
    void PassEvent(void* a_event);

    std::shared_ptr<Object> m_rootNode;

    std::weak_ptr<Object> FindObject(unsigned int a_id);
    void FindObjectsByName
      (std::string a_name, std::vector<std::weak_ptr<Object>>& a_objectVec);

    static int ScreenWidth, ScreenHeight;
  private:
    Core();
    void HandleActions();
  };
};

#define GameLayerCore GameLayer::Core::GetInstance()
